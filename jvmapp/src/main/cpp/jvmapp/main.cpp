#include <jni.h>

#include <cstddef>

#include <dlfcn.h>

#include <iostream>
#include <string>
#include <vector>

#include "jvmapp/config.hpp"

namespace {
    void _onError(const std::string& msg) noexcept {
        std::cerr << "[jni] error: " << msg << std::endl;
        __builtin_trap();
    }

    void _onWarning(const std::string& msg) noexcept {
        std::cerr << "[jni] warning: " << msg << std::endl;
    }
}

int main(int argc, char ** argv) {
    auto jvmopts = std::vector<JavaVMOption>();

    jvmopts.reserve(jvmapp::JAVA_VM_ARG_COUNT);

    for (std::size_t i = 0; i < jvmapp::JAVA_VM_ARG_COUNT; i++) {
        auto opt = JavaVMOption{
            .optionString = const_cast<char *> (reinterpret_cast<const char *> (jvmapp::JAVA_VM_ARGS[i].c_str())),
            .extraInfo = nullptr};

        jvmopts.push_back(opt);
    }

    auto vmArgs = JavaVMInitArgs{
        .version = JNI_VERSION_1_8,
        .nOptions = static_cast<jint> (jvmopts.size()),
        .options = jvmopts.data(),
        .ignoreUnrecognized = JNI_TRUE};
        
    JavaVM * pJavaVM;
    JNIEnv * pJNIEnv;

    auto pJVMLib = dlopen("../lib/server/libjvm.so", RTLD_LAZY);
    
    {
        auto error = dlerror();

        if (error) {
            _onError(error);
        }
    }

    typedef jint(*CreateJvmFuncPtr) (JavaVM**, void**, JavaVMInitArgs*);

    auto pfnJNI_CreateJVM = reinterpret_cast<CreateJvmFuncPtr> (dlsym(pJVMLib, "JNI_CreateJavaVM"));

    auto flag = pfnJNI_CreateJVM(&pJavaVM, reinterpret_cast<void **> (&pJNIEnv), &vmArgs);

    if (flag == JNI_ERR) {
        _onError("Error creating JavaVM!");
    }

    auto jmain = pJNIEnv->FindClass(jvmapp::JAVA_MAIN.c_str());

    if (jmain == nullptr) {
        pJNIEnv->ExceptionDescribe();
        pJavaVM->DestroyJavaVM();
    } else {
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

    dlclose(pJVMLib);

    return 0;
}