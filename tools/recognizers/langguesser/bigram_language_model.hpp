/*
  Copyright (C) 2007-2011 Poleng Sp. z o.o.

  This file is part of Translatica language identification module.

  Translatica language identification module (along with bigram
  tables) can be redistributed and/or modified under the terms of the
  GNU Lesser General Public Licence as published by the Free Software
  Foundation.
*/

#ifndef BIGRAM_LANGUAGE_MODEL_HDR
#define BIGRAM_LANGUAGE_MODEL_HDR

#include <boost/filesystem/path.hpp>
#include <string>

class BigramLanguageModel {

public:

    static const int TABLE_SIZE = 512;

    BigramLanguageModel(std::string& text);
    BigramLanguageModel(boost::filesystem::path file);

    double* frequencyTable();

private:

    static const int FACTOR = 509;

    double table_[TABLE_SIZE];

    void countBigramsInText(std::string& text, int* countTable);
    void createFrequencyTableFromCountTable(int* countTable);
    int sumOfCounts(int* countTable);
    void zeroTable();


};

#endif
