#ifndef GUID_GENERATOR_HDR
#define GUID_GENERATOR_HDR

#include <string>

class GUIDGenerator {

public:

    GUIDGenerator(int length, bool with_date);

    std::string getGUID();

private:

    int _length;
    bool _withDate;
    int _randomLength;
    int _counter;

    std::string randomString(int len);
    std::string currentDateTime();

    std::string nextNumber();
    std::string zeroPadNumber(int num);
    
    int determineLength(int len);
    int determineRandomLength();

};

#endif
