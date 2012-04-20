#ifndef BILEXICON_HDR_HDR
#define BILEXICON_HDR_HDR

#include "lattice.hpp"
#include "key_value_store.hpp"
#include "utt_like_quoter.hpp"
#include "annotator_factory.hpp"

#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

class BiLexicon {

public:
    BiLexicon(const boost::program_options::variables_map& options);

    static std::string getName();
    static boost::filesystem::path getFile();

    static AnnotatorFactory::LanguagesHandling languagesHandling(
        const boost::program_options::variables_map& options);

    static std::list<std::string> languagesHandled(
        const boost::program_options::variables_map& options);

    static boost::program_options::options_description optionsHandled();
    static std::list<std::string> providedLayerTags();
    static std::list<std::string> tagsToOperateOn();

    void processEdge(Lattice& lattice, Lattice::EdgeDescriptor edge);

    class Exception : public PsiException  {
    public:
        Exception(const std::string& msg): PsiException(msg) {
        }

        virtual ~Exception() throw() {}
    };

private:
    void readPlainText_(const boost::filesystem::path& plainTextLexicon);
    void saveBinary_(const boost::filesystem::path& binaryLexiconPath);
    void loadBinary_(const boost::filesystem::path& binaryLexiconPath);

    void addEntry_(Lattice& lattice, Lattice::EdgeDescriptor edge, const std::string& record);
    AnnotationItem parseRecord_(const std::string& record);

    void removeComment_(std::string& s);
    bool isEmptyLine_(const std::string& s);

    void createTags_(const std::string& trg_lang);

    boost::shared_ptr<KeyValueStore> store_;
    UTTLikeQuoter quoter_;
    std::list<std::string> tags_;

    static const std::string DEFAULT_BINARY_LEXICON_SPEC;
};

#endif
