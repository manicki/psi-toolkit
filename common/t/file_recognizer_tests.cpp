#include "tests.hpp"
#include "logging.hpp"
#include <fstream>

#include "file_recognizer.hpp"

BOOST_AUTO_TEST_SUITE( file_recognizer )

BOOST_AUTO_TEST_CASE( recognize_text_file ) {
    FileRecognizer fileRecognizer;

    std::string content = "Ala ma kota i psa";

    BOOST_CHECK_EQUAL(fileRecognizer.recognizeMimeType(content), "text");
    BOOST_CHECK_EQUAL(fileRecognizer.recognizeFileExtension(content), "txt");

    content = std::string("<html><body><p>\n") + content + "</p></body></html>";

    BOOST_CHECK_EQUAL(fileRecognizer.recognizeMimeType(content), "text");
    BOOST_CHECK_EQUAL(fileRecognizer.recognizeFileExtension(content), "html");

    std::string type;
    std::string extension;
    fileRecognizer.recognizeMimeTypeAndFileExtension(content, type, extension);

    BOOST_CHECK_EQUAL(type, "text");
    BOOST_CHECK_EQUAL(extension, "html");
}

BOOST_AUTO_TEST_CASE( recognize_psi_format ) {
    FileRecognizer fileRecognizer;

    std::ifstream ifs(ROOT_DIR "common/t/files/pl_sample_simple.psi");
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));

    BOOST_CHECK_EQUAL(fileRecognizer.recognizeMimeType(content), "text");
    //FIXME: memory access violation error - because of regexp?
    //BOOST_CHECK_EQUAL(fileRecognizer.recognizeFileExtension(content), "psi");
}

BOOST_AUTO_TEST_CASE( recognize_utt_format ) {
    FileRecognizer fileRecognizer;

    std::ifstream ifs(ROOT_DIR "common/t/files/fr_simple_puddle_input.utt");
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));

    BOOST_CHECK_EQUAL(fileRecognizer.recognizeMimeType(content), "text");
    //FIXME: memory access violation error - because of regexp?
    //BOOST_CHECK_EQUAL(fileRecognizer.recognizeFileExtension(content), "utt");
}

BOOST_AUTO_TEST_SUITE_END()
