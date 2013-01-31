/*
  Copyright (C) 2007-2011 Poleng Sp. z o.o.

  This file is part of Translatica language identification module.

  Translatica language identification module (along with bigram
  tables) can be redistributed and/or modified under the terms of the
  GNU Lesser General Public Licence as published by the Free Software
  Foundation.
*/

#ifndef LANG_GUESSER_HDR
#define LANG_GUESSER_HDR

#include "bigram_language_model.hpp"

#include "annotator.hpp"
#include "annotator_factory.hpp"
#include "lattice_wrapper.hpp"
#include "processor_file_fetcher.hpp"
#include "logging.hpp"

#include <boost/program_options/variables_map.hpp>
#include <boost/filesystem.hpp>

#include <string>
#include <list>
#include <map>

class LangGuesser : public Annotator {

public:

    LangGuesser();
    LangGuesser(const boost::program_options::variables_map& options);

    std::string guessLanguage(std::string text);
    std::string guessLanguageByBigramModel(std::string text);
    std::string guessLanguageByLetters(std::string text);

    class Factory : public AnnotatorFactory {
    public:
        virtual Annotator* doCreateAnnotator(
            const boost::program_options::variables_map& options);

        virtual std::string doGetName() const;
        virtual boost::filesystem::path doGetFile() const;
        virtual boost::program_options::options_description doOptionsHandled();

        LanguagesHandling doLanguagesHandling(
            const boost::program_options::variables_map& options) const;
        std::list<std::string> doLanguagesHandled(
            const boost::program_options::variables_map& options) const;

        virtual std::list<std::list<std::string> > doRequiredLayerTags();
        virtual std::list<std::list<std::string> > doOptionalLayerTags();
        virtual std::list<std::string> doProvidedLayerTags();
    };

    class Exception : public PsiException  {
    public:
        Exception(const std::string& msg): PsiException(msg) {
        }

        virtual ~Exception() throw() {}
    };

private:

    static const unsigned int MIN_TEXT_LENGTH_FOR_BIGRAM_METHOD = 24;
    static std::string UNKNOWN_LANGUAGE;
    static std::string DEFAULT_DEFAULT_LANGUAGE;
    static std::string NONE_LANGUAGE;
    static std::map<std::string, std::string> LANGUAGES;

    struct Language {
        std::string name;
        BigramLanguageModel model;
        std::string letters;

        Language(std::string lang, boost::filesystem::path file, std::string specialLetters)
            : name(lang), model(file), letters(specialLetters) { };
    };

    std::list<Language> languages_;
    bool forceMode_;
    std::string defaultLanguage_;
    std::string forcedLanguage_;

    void initLanguages();
    void initLanguages(std::vector<std::string> selectedLangs);
    void addLanguage(std::string lang, std::string letters);

    bool isOneOfTheLanguageSpecificLetters(utf8::uint32_t letter, std::string& letters);
    double distance(double* ftableOne, double* ftableTwo);

    class Worker : public LatticeWorker {
    public:
        Worker(LangGuesser& processor, Lattice& lattice);
    private:
        virtual void doRun();

        void markLanguage_(const std::string& language, Lattice::EdgeDescriptor edge);
        LayerTagCollection getTagForLanguage_(const std::string& language);

        bool guessLanguage_();

        LangGuesser& processor_;
    };

    virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice);
    virtual std::string doInfo();

};

#endif
