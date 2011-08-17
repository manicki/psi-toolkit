#include "java_virtual_machine.hpp"

class JavaVirtualMachineTests : public CxxTest::TestSuite {

	public:

	void testSingleton() {
		//JavaVirtualMachine::setJavaClassPath("../common/j");
		JavaVirtualMachine *jvm1 = JavaVirtualMachine::Instance();
  	    JavaVirtualMachine *jvm2 = jvm1->Instance();

		TS_ASSERT_EQUALS( (long)jvm1, (long)jvm2 );
		//delete jvm1;
		//delete jvm2;
	}

	void testHelloWorld() {
		JavaVirtualMachine * jvm = JavaVirtualMachine::Instance();
		jclass cls = NULL;
		jmethodID mid = NULL;
		const char * hello = NULL;

		JNIEnv * env = jvm->getENV();
		TS_ASSERT(env != NULL);

		cls = env->FindClass("HelloWorld");
		TS_ASSERT(cls != NULL);

		if (cls != NULL) {
			mid = env->GetStaticMethodID(cls, "getHello", "()Ljava/lang/String;");
		}		
		TS_ASSERT(mid != NULL);

		if (mid != NULL) {
			jstring jstr = (jstring)env->CallStaticObjectMethod(cls, mid, NULL);
			hello = env->GetStringUTFChars(jstr,0);
		}
		TS_ASSERT_EQUALS((std::string)hello, "Hello World from Java");
	}

};

