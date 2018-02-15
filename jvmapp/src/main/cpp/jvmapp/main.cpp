#include "pch.h"
#include <jni.h>

#include <cstddef>
#include <cstdio>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <direct.h>
#define GetCurrentDir _getcwd
#elif defined(__linux__)
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include "jvmapp/config.hpp"
#include "jvmapp/shared_object.hpp"

namespace {
    std::string GetCurrentWorkingDir() {
        char buff[FILENAME_MAX];

        GetCurrentDir(buff, FILENAME_MAX);

        return std::string(buff);
    }

    std::string getProgramPath(const std::string& argv0) {
        std::string absPath;
        
        {
            std::stringstream builder;

            builder << GetCurrentWorkingDir();

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
            builder << "\\";
#elif defined(__linux__)
            builder << "/";
#endif

            builder << argv0;

            absPath = builder.str();
        }

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        auto pos = absPath.rfind('\\');
#elif defined(__linux__)
        auto pos = absPath.rfind('/');
#endif

        return absPath.substr(0, pos+1);
    }

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
    auto absPath = getProgramPath(argv[0]);
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
    
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    _jvmso = jvmapp::shared_object(absPath + "../lib/server/jvm.dll");
#elif defined(__linux__)
    _jvmso = jvmapp::shared_object(absPath + "../lib/server/libjvm.so");
#endif

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