#include "tests.hpp"

#include "../java_virtual_machine.hpp"

BOOST_AUTO_TEST_SUITE( java_virtual_machine )

BOOST_AUTO_TEST_CASE( java_singleton ) {
    JavaVirtualMachine *jvm1 = JavaVirtualMachine::Instance();
    JavaVirtualMachine *jvm2 = jvm1->Instance();

    BOOST_CHECK_EQUAL( (long)jvm1, (long)jvm2 );
    //delete jvm1;
    //delete jvm2;
}

BOOST_AUTO_TEST_CASE( java_hello_world ) {
    JavaVirtualMachine * jvm = JavaVirtualMachine::Instance();
    jclass cls = NULL;
    jmethodID mid = NULL;
    const char * hello = NULL;

    JNIEnv * env = jvm->getENV();
    BOOST_CHECK(env != NULL);

    cls = env->FindClass("HelloWorld");
    BOOST_CHECK(cls != NULL);

    if (cls != NULL) {
        mid = env->GetStaticMethodID(cls, "getHello", "()Ljava/lang/String;");
    }
    BOOST_CHECK(mid != NULL);

    if (mid != NULL) {
        jstring jstr = (jstring)env->CallStaticObjectMethod(cls, mid, NULL);
        hello = env->GetStringUTFChars(jstr,0);
    }
    BOOST_CHECK_EQUAL((std::string)hello, "Hello World from Java");
}

BOOST_AUTO_TEST_SUITE_END()
