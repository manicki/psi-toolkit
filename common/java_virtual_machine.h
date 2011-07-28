#include <jni.h>
#include <string>

using std::string;

class JavaVirtualMachine {

	public:
	static jint const JNI_VERSION;
	static string javaClassPath;
	static string javaLibraryPath;

	JavaVM* getJVM();
	JNIEnv* getENV();
	static void setJavaPaths(string classPath, string libraryPath);

	static JavaVirtualMachine* Instance();

	private:
	bool inited;
	JavaVM* javaVM;
	JNIEnv* jniENV;
	JNIEnv* create(JavaVM ** jvm);

	static JavaVirtualMachine* jvmInstance;
  JavaVirtualMachine();
  JavaVirtualMachine(const JavaVirtualMachine&);
  JavaVirtualMachine& operator= (const JavaVirtualMachine&);

};
