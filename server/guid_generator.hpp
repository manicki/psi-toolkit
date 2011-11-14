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

    int determineLength(int len);
    int determineRandomLength();
    std::string zeroPadNumber(int num);
};
