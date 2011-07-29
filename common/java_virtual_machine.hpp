#ifndef JAVA_VIRTUAL_MACHINE_HDR
#define JAVA_VIRTUAL_MACHINE_HDR

#include <jni.h>
#include <string>

using std::string;

class JavaVirtualMachine {

	public:
	static jint const JNI_VERSION;

	JavaVM* getJVM();
	JNIEnv* getENV();
	static void setJavaClassPath(string);
    static void setJavaLibraryPath(string);

	static JavaVirtualMachine* Instance();

	private:
	static string javaClassPath;
	static string javaLibraryPath;
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
