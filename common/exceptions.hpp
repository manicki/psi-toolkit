#ifndef PsiExceptionS_HDR
#define PsiExceptionS_HDR


#include <string>

#include "psi_exception.hpp"


class NoEdgeException : public PsiException {
public:
    NoEdgeException(std::string message) : PsiException(message) { }
};


class NoVertexException : public PsiException {
public:
    NoVertexException(std::string message) : PsiException(message) { }
};


class WrongVertexException : public PsiException {
public:
    WrongVertexException(std::string message) : PsiException(message) { }
};


class FileFormatException : public PsiException {
public:
    FileFormatException(std::string message) : PsiException(message) { }
};


class QuoterException : public PsiException {
public:
    QuoterException(std::string message) : PsiException(message) { }
};


class JavaVirtualMachineException : public PsiException {
public:
    JavaVirtualMachineException(std::string message) : PsiException(message) { }
};


class StringFragException : public PsiException {
public:
    StringFragException(std::string message) : PsiException(message) { }
};


class EdgeSelfReferenceException : public PsiException {
public:
    EdgeSelfReferenceException(std::string message) : PsiException(message) { }
};


class ReversedEdgeException : public PsiException {
public:
    ReversedEdgeException(std::string message) : PsiException(message) { }
};


class EdgeInsertionException : public PsiException {
public:
    EdgeInsertionException(std::string message) : PsiException(message) { }
};


#endif
