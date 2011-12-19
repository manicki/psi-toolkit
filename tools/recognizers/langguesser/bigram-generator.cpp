#include "bigram_language_model.hpp"

#include "logging.hpp"

int main()
{
    INFO("Put the text in one language on the basis of which "
        "the bigram language model will be generated:");

    std::string text;
    std::string input;

    while (std::cin >> input) {
        text += input + " ";
    }

    BigramLanguageModel model(text);

    for (int i = 0; i < BigramLanguageModel::TABLE_SIZE; i++) {
        std::cout << model.frequencyTable()[i] << std::endl;
    }

    return 0;
}
