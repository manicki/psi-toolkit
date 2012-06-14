#ifndef LEXICON_BASE_HDR
#define LEXICON_BASE_HDR

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>

#include "key_value_store.hpp"
#include "utt_like_quoter.hpp"

class LexiconBase {
public:
    void readPlainText(const boost::filesystem::path& plainTextLexicon);
    void saveBinary(const boost::filesystem::path& binaryLexiconPath);
    void loadBinary(const boost::filesystem::path& binaryLexiconPath);
    bool isEmpty();
    std::vector<std::string> getRecords(const std::string& text);

    class Exception : public PsiException  {
    public:
        Exception(const std::string& msg): PsiException(msg) {
        }

        virtual ~Exception() throw() {}
    };

private:
    void removeComment_(std::string& s);
    bool isEmptyLine_(const std::string& s);

    boost::shared_ptr<KeyValueStore> store_;
    UTTLikeQuoter quoter_;
};

#endif
