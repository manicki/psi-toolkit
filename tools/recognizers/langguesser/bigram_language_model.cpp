/*
  Copyright (C) 2007-2011 Poleng Sp. z o.o.

  This file is part of Translatica language identification module.

  Translatica language identification module (along with bigram
  tables) can be redistributed and/or modified under the terms of the
  GNU Lesser General Public Licence as published by the Free Software
  Foundation.
*/

#include "bigram_language_model.hpp"

#include "processor_file_fetcher.hpp"
#include "utf8/utf8.h"

#include <iostream>
#include <fstream>
#include <sstream>

#define LOWER_LETTER(i) ( ((i) > 90 || (i) < 65) ? (i) : ((i) + 32) )

BigramLanguageModel::BigramLanguageModel(std::string& text) {

    int countTable[TABLE_SIZE] = {0};
    countBigramsInText(text, countTable);
    createFrequencyTableFromCountTable(countTable);
}

BigramLanguageModel::BigramLanguageModel(boost::filesystem::path pathToLangFile) {

    std::ifstream langFile(pathToLangFile.string().c_str(), std::ifstream::in);

    int i = 0;
    while (langFile.good() && i < TABLE_SIZE) {
        langFile >> table_[i++];
    }

    langFile.close();
}

double* BigramLanguageModel::frequencyTable() {
    return table_;
}

void BigramLanguageModel::countBigramsInText(std::string& text, int* countTable) {

    if (text.length() < 2) {
        return;
    }

    std::string::iterator iter = text.begin();
    std::string::iterator end = text.end();

    utf8::uint32_t a = utf8::next(iter, end);
    a = LOWER_LETTER(a);
    utf8::uint32_t b = utf8::next(iter, end);
    b = LOWER_LETTER(b);

    while (iter != end) {
        ++countTable[(a * FACTOR + b * (FACTOR * FACTOR)) % TABLE_SIZE];
        a = b;
        b = utf8::next(iter, end);
        b = LOWER_LETTER(b);
    }
}

void BigramLanguageModel::createFrequencyTableFromCountTable(int* countTable) {
    int sum = sumOfCounts(countTable);

    if (sum == 0) {
        zeroTable();
    }
    else {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table_[i] = double(countTable[i]) / double(sum);
        }
    }
}

int BigramLanguageModel::sumOfCounts(int* countTable) {
    int sum = 0;

    for (int i = 0; i < TABLE_SIZE; i++) {
        sum += countTable[i];
    }

    return sum;
}

void BigramLanguageModel::zeroTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table_[i] = 0.0;
    }
}
