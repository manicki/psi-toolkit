#include "guid_generator.hpp"

#include <sstream>
#include <iomanip>
#include <cstdlib>

#define GUID_MIN_SIZE 16
#define GUID_DATE_SIZE 7
#define GUID_PAD_SIZE 4
#define GUID_SEPARATOR '-'

GUIDGenerator::GUIDGenerator(int length, bool with_date)
 : _length(determineLength(length)),
   _withDate(with_date),
   _randomLength(determineRandomLength()),
   _counter(0) {  }

std::string GUIDGenerator::getGUID()
{
    std::string guid = "";

    if (_withDate) {
        guid += currentDateTime();
        guid += GUID_SEPARATOR;
    }
    guid += nextNumber();
    guid += GUID_SEPARATOR;

    guid += randomString(_randomLength);

    return guid;
}

std::string GUIDGenerator::nextNumber()
{
    return zeroPadNumber(++_counter);
}

std::string GUIDGenerator::currentDateTime()
{
    char buff[GUID_DATE_SIZE];
    time_t now = time(NULL);
    strftime(buff, GUID_DATE_SIZE, "%y%m%d", localtime(&now));

    return std::string(buff);
}

std::string GUIDGenerator::randomString(int len)
{
    std::string s = "";
    srand(time(NULL) + _counter);

    for (int i = 0; i < len; ++i) {
        int randomChar = rand()%(26+26+10);

        if (randomChar < 26)
            s += 'a' + randomChar;
        else if (randomChar < 26+26)
            s += 'A' + randomChar - 26;
        else
            s += '0' + randomChar - 26 - 26;
    }

    return s;
}

std::string GUIDGenerator::zeroPadNumber(int num)
{
    std::ostringstream ss;

    ss << std::setw(GUID_PAD_SIZE) << std::setfill('0') << num;
    std::string result = ss.str();

    if (result.length() > GUID_PAD_SIZE) {
        result.erase(0, result.length() - GUID_PAD_SIZE);
    }

    return result;
}

int GUIDGenerator::determineLength(int len)
{
    return len < GUID_MIN_SIZE ? GUID_MIN_SIZE : len;
}

int GUIDGenerator::determineRandomLength() {
    int currSize = _withDate ? GUID_PAD_SIZE + GUID_DATE_SIZE + 2 : GUID_PAD_SIZE + 1;
    return _length - currSize;
}
