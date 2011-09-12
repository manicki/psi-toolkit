#ifndef TP_BASIC_TOKENIZER_RULE_SET_HDR
#define TP_BASIC_TOKENIZER_RULE_SET_HDR

#include <map>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <exception>

#include <boost/shared_ptr.hpp>

#include "regexp.hpp"

class TPTokenizerException: public std::exception {
    std::string cause;
public:
    TPTokenizerException(std::string c) {
        cause = c;
    }

    virtual const char* what() const throw() {
        return cause.c_str();
    }

    virtual ~TPTokenizerException() throw() {};
};

class TPBasicTokenizerRuleSet {

public:
    PerlRegExp* getMainRegex();
    PerlRegExp* getRegex(size_t i);
    PerlRegExp* getRegex(std::string &name);
    std::string getRegexName(size_t i) const;
    std::string getRegexCategory(size_t i) const;
    std::string getRegexCategory(std::string &name) const;
    size_t getRegexCount() const;

    void addRegex(boost::shared_ptr<PerlRegExp> regex, std::string &category, std::string &name);
    bool load(std::string fileName);

    TPBasicTokenizerRuleSet();
    ~TPBasicTokenizerRuleSet();

    void load(std::map<std::string, std::string> & paths, std::list<std::string> & filesToLoad );

private:
    struct _regex {
        std::string category;
        std::string name;
        boost::shared_ptr<PerlRegExp> reg;
    };

    struct stackElem {
        size_t epos;
        size_t len;
        std::string name;

        stackElem(size_t p, size_t le, std::string n):epos(p),len(le),name(n) {
        }
    };

    enum IfTypes {
        TIF,
        TELSE
    };

    class IfElem {
        bool value;
        std::string context;
        size_t pos;
        IfTypes type;

    public:
        IfElem(bool val, std::string cont, size_t p, IfTypes t):
            value(val), context(cont), pos(p), type(t) {
        };

        IfTypes getType() const {
            return type;
        }

        void setType(IfTypes t) {
            type = t;
        }

        bool getValue() const {
            return value;
        }

        void setValue(bool val) {
            value = val;
        }

        std::string getContext() const {
            return context;
        }

        void setContext(std::string& con) {
            context = con;
        }

        size_t getPos() const {
            return pos;
        }

        void setPos(size_t& p) {
            pos = p;
        }
    };

    class Definition {
        std::string value;
        std::string context;
    public:
        Definition(std::string val, std::string con)
            : value(val), context(con) {
        }

        std::string getValue() const {
            return value;
        }

        void setValue(std::string & val) {
            value = val;
        }

        std::string getContext() const {
            return context;
        }

        void setContext(std::string & con) {
            context = con;
        }
    };

    bool openedIf;
    std::string  itoa(size_t n);

    void subLoad(std::map<std::string, std::string> & paths,
                 std::map<std::string, Definition> & defs,
                 std::string context,
                 std::map<std::string, bool> & opened,
                 std::string& toLoad);

    void parseLine(std::string& resbuffer,
                   std::map<std::string, Definition> & defs,
                   std::map<std::string, std::string> & paths,
                   std::string context,
                   std::map<std::string, bool> & opened,
                   size_t & filePos,
                   std::list<IfElem> & ifstack);

    void readDefinition(std::string & resbuffer, std::string context,
                        size_t filePos, std::map<std::string, Definition> & defs);

    void readInclude(std::string & resbuffer,
                     std::map<std::string, std::string> & paths,
                     std::string &context,
                     std::map<std::string, bool> & opened,
                     size_t filePos,
                     std::map<std::string, Definition> & defs);

    void readRule(std::string & resbuffer,
                  std::string context,
                  size_t filePos);

    std::string applyDefinitions(std::string & buffer,
                                 std::map<std::string, Definition> & defs,
                                 std::string & subst);

    bool changeHereDoc(std::string & strg);

    // returns true if the following characters would be part of a comment
    bool removeComments(std::string & comm);

    void trim(std::string& strg);

    void loadWholeElement(std::istream & input, std::string & buff, size_t & pos);
    void loadWholeElement(std::istream & input, std::string & buff);

    void clearState(std::map<std::string, Definition> & definitions, std::map<std::string, bool> & opened);
    void returnToTheBuffer(std::stringstream & st, std::string & resBuffer);
    std::string getContent(std::string & buffer,std::string  pattern,
                             std::string msg, std::string & context, size_t filePos);

    void isEmpty(std::string & buffer, std::string msg, std::string & context, size_t filePos);

    void readIfNDef(std::string& resbuffer, std::string context,size_t & filePos,
                    std::map<std::string, Definition> & defs,  std::list<IfElem> &ifstack);

    void readIfDef(std::string& resbuffer, std::string context,size_t & filePos,
                   std::map<std::string, Definition> & defs,  std::list<IfElem> &ifstack);

    void readIf(
        std::string& resbuffer, std::string context,size_t & filePos,
        std::map<std::string, Definition> & defs,  std::list<IfElem> &ifstack,
        bool expectation);

    void readElse(std::string context,size_t & filePos,
                  std::list<IfElem> &ifstack);

    void readEndIf(std::list<IfElem> &ifstack);

    std::vector<_regex> regs;
    boost::shared_ptr<PerlRegExp> mainRegex;

    const static size_t MAXSTACKSIZE;

    const static std::string MAIN_REGEX_NAME;
};

#endif

