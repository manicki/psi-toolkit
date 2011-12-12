#include <string>

class ModelLanguage {

public:

    static const int FTABLE_LENGTH = 512;

    ModelLanguage(std::string lang, std::string file, std::string nonStandardLetters);

    const std::string name;
    const std::string letters;
    double bigramFrequencyTable[FTABLE_LENGTH];

private:

    void initBigramFrequencyTable(std::string file);

};
