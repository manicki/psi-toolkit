#ifndef JAVA_VIRTUAL_MACHINE_HDR
#define JAVA_VIRTUAL_MACHINE_HDR

#include <jni.h>
#include <string>
#include "exceptions.hpp"

class JavaVirtualMachine {

public:
	static jint const JNI_VERSION;

	JavaVM* getJVM();
	JNIEnv* getENV();
	static void addJavaClassPath(std::string);
    static void addJavaLibraryPath(std::string);

	static JavaVirtualMachine* Instance();

private:
	static std::string javaClassPath;
	static std::string javaLibraryPath;
	bool inited;
	JavaVM* javaVM;
	JNIEnv* jniENV;
	JNIEnv* create(JavaVM ** jvm);

	static JavaVirtualMachine* jvmInstance;
    JavaVirtualMachine();
    JavaVirtualMachine(const JavaVirtualMachine&);
    JavaVirtualMachine& operator= (const JavaVirtualMachine&);

};
#endif
