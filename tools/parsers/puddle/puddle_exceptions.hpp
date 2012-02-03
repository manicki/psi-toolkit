#ifndef PUDDLE_EXCEPTIONS_HDR
#define PUDDLE_EXCEPTIONS_HDR

#include "psi_exception.hpp"

class PuddleRuleElementNotMatchedException : public PsiException {
    public:
        PuddleRuleElementNotMatchedException(std::string message) :
            PsiException(message) { }
};

class PuddleNoLemmaEdgeException : public PsiException {
    public:
        PuddleNoLemmaEdgeException(std::string message) :
            PsiException(message) { }
};

class PuddleNoLexemeEdgeException : public PsiException {
    public:
        PuddleNoLexemeEdgeException(std::string message) :
            PsiException(message) { }
};

class PuddleNoMatchingEdgeException : public PsiException {
    public:
        PuddleNoMatchingEdgeException(std::string message) :
            PsiException(message) { }
};

class PuddleNoTagsetException : public PsiException {
    public:
        PuddleNoTagsetException(std::string message) :
            PsiException(message) { }
};

class PuddleNoRulesException : public PsiException {
    public:
        PuddleNoRulesException(std::string message) :
            PsiException(message) { }
};

class PuddleRuleSyntaxException : public PsiException {
    public:
        PuddleRuleSyntaxException(std::string message) :
            PsiException(message) { }
};

class PuddleTagsetSyntaxException : public PsiException {
    public:
        PuddleTagsetSyntaxException(std::string message) :
            PsiException(message) { }
};

class PuddleIOException : public PsiException {
    public:
        PuddleIOException(std::string message) :
            PsiException(message) { }
};


#endif
