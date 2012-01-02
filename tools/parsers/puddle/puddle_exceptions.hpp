#ifndef PUDDLE_EXCEPTIONS_HDR
#define PUDDLE_EXCEPTIONS_HDR

#include "exceptions.hpp"

class PuddleRuleElementNotMatchedException : public Exception {
    public:
        PuddleRuleElementNotMatchedException(std::string message) :
            Exception(message) { }
};

class PuddleNoLemmaEdgeException : public Exception {
    public:
        PuddleNoLemmaEdgeException(std::string message) :
            Exception(message) { }
};

class PuddleNoLexemeEdgeException : public Exception {
    public:
        PuddleNoLexemeEdgeException(std::string message) :
            Exception(message) { }
};

class PuddleNoMatchingEdgeException : public Exception {
    public:
        PuddleNoMatchingEdgeException(std::string message) :
            Exception(message) { }
};

class PuddleNoTagsetException : public Exception {
    public:
        PuddleNoTagsetException(std::string message) :
            Exception(message) { }
};

class PuddleNoRulesException : public Exception {
    public:
        PuddleNoRulesException(std::string message) :
            Exception(message) { }
};

class PuddleRuleSyntaxException : public Exception {
    public:
        PuddleRuleSyntaxException(std::string message) :
            Exception(message) { }
};

class PuddleTagsetSyntaxException : public Exception {
    public:
        PuddleTagsetSyntaxException(std::string message) :
            Exception(message) { }
};

class PuddleIOException : public Exception {
    public:
        PuddleIOException(std::string message) :
            Exception(message) { }
};


#endif
