#ifndef HDR_HDR_GRPARSER
#define HDR_HDR_GRPARSER
/**
 * \file grparser.h
 */

#include "grules.h"

class GRParser
{
public:
    GRParser() {}
    ~GRParser() {}

    GRuleList* parse(istream& a_input_stream, bool a_encoded);

    class ParseError: public exception
    {
    public:
        int line_no;
        string message;
        ParseError(int a_line_no, string a_message)
        :line_no(a_line_no), message(a_message) {}
        ~ParseError() throw() {}
    };
};

#endif
