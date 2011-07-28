#include "java_virtual_machine.cpp"

class JavaVirtualMachineTests : public CxxTest::TestSuite {

	void testSingleton() {
		JavaVirtualMachine *jvm1 = JavaVirtualMachine::Instance();
  	JavaVirtualMachine *jvm2 = jvm1->Instance();

		//ASSERT_EQUALS();

		delete jvm1;
		delete jvm2;
	}
	
	void testHelloWorldFromJava() {
		JavaVirtualMachine * jvm = JavaVirtualMachine::Instance();
		JNIEnv * e = jvm1->getENV();

		jclass cls = NULL;
		jmethodID mid = NULL;
		const char * hello;

		ASSERT(cls);
		if (cls != NULL) {
			mid = e->GetStaticMethodID(cls, "getHello", "()Ljava/lang/String;");
		}
		
		ASSERT(mid);
		if (mid != NULL) {
			jstring jstr = (jstring)e->CallStaticObjectMethod(cls, mid, NULL);
			hello = e->GetStringUTFChars(jstr,0);
		}

		TS_ASSERT_EQUALS(hello, "Hello World from Java!");
		delete mid;
		delete cls;
		delete e;
		delete jvm;
	}

}

