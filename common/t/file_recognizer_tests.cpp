#include "tests.hpp"

#include "file_recognizer.hpp"

BOOST_AUTO_TEST_SUITE( file_recognizer )

BOOST_AUTO_TEST_CASE( recognize_text_file ) {
    FileRecognizer fileRecongnizer;

    std::string content = "Ala ma kota i psa";
    BOOST_CHECK_EQUAL(fileRecongnizer.recognizeMimeType(content), "text");
    BOOST_CHECK_EQUAL(fileRecongnizer.recognizeFileExtension(content), "txt");
}

BOOST_AUTO_TEST_SUITE_END()
