#include "pch.h"
#include <jni.h>

#include <cstddef>

#include "jvmapp/config.hpp"
#include "jvmapp/shared_object.hpp"

namespace {
    /*
    * Class:     jvmapp_Demo
    * Method:    testReentrant
    * Signature: ()V
    */
    JNIEXPORT void JNICALL _testReentrant(JNIEnv *, jclass) {
        std::cout << "Hello from C++! This method isn't even exported." << std::endl;
    }

    void _onError(const std::string& msg) noexcept {
        std::cerr << "[jni] error: " << msg << std::endl;
        __builtin_trap();
    }

    void _onWarning(const std::string& msg) noexcept {
        std::cerr << "[jni] warning: " << msg << std::endl;
    }

    jvmapp::shared_object _jvmso;
}

int main(int argc, char ** argv) {
    auto jvmopts = std::vector<JavaVMOption>();

    jvmopts.reserve(jvmapp::JAVA_VM_ARG_COUNT);

    for (std::size_t i = 0; i < jvmapp::JAVA_VM_ARG_COUNT; i++) {
        auto opt = JavaVMOption{
            const_cast<char *> (reinterpret_cast<const char *> (jvmapp::JAVA_VM_ARGS[i].c_str())),
            nullptr};

        jvmopts.push_back(opt);
    }

    auto vmArgs = JavaVMInitArgs{
        JNI_VERSION_9,
        static_cast<jint> (jvmopts.size()),
        jvmopts.data(),
        JNI_TRUE};
        
    JavaVM * pJavaVM;
    JNIEnv * pJNIEnv;

    _jvmso = jvmapp::shared_object("../lib/server/libjvm");

    auto pfnJNI_CreateJVM = _jvmso.loadPfn<JNICALL jint(JavaVM**, void**, void*)>("JNI_CreateJavaVM");
    auto flag = pfnJNI_CreateJVM(&pJavaVM, reinterpret_cast<void **> (&pJNIEnv), &vmArgs);

    if (flag == JNI_ERR) {
        _onError("Error creating JavaVM!");
    }

    auto jmain = pJNIEnv->FindClass(jvmapp::JAVA_MAIN.c_str());

    if (jmain == nullptr) {
        pJNIEnv->ExceptionDescribe();
        pJavaVM->DestroyJavaVM();
    } else {        
        JNINativeMethod pfnTestReentrantInfo = {
            const_cast<char *> ("testReentrant"),
            const_cast<char *> ("()V"),
            reinterpret_cast<void *> (&_testReentrant)};

        pJNIEnv->RegisterNatives(jmain, &pfnTestReentrantInfo, 1);

        auto mainId = pJNIEnv->GetStaticMethodID(jmain, "main", "([Ljava/lang/String;)V");

        if (mainId != nullptr) {            
            auto jStringClass = pJNIEnv->FindClass("java/lang/String");
            auto argInitValue = pJNIEnv->NewStringUTF("");
            auto jargs = pJNIEnv->NewObjectArray(argc - 1, jStringClass, argInitValue);

            for (decltype(argc) i = 1; i < argc; i++) {
                auto jarg = pJNIEnv->NewStringUTF(argv[i]);

                pJNIEnv->SetObjectArrayElement(jargs, i - 1, jarg);
            }

            pJNIEnv->CallStaticVoidMethod(jmain, mainId, jargs);

            if (pJavaVM->DetachCurrentThread() != 0) {
                _onWarning("Could not detach main thread!");
            }
        } else {
            _onError("Unable to find entry point!");
        }        
    }

    pJavaVM->DestroyJavaVM();

    return 0;
}