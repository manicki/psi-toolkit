#include "grparser.h"

#include <string.h>

extern GRuleList* grparser_all;
extern char grerror_message[];
extern int grlexlineno;


int grparserparse();

std::istream* grlex_input_stream;

extern bool first_read;
extern bool encoded;

GRuleList* GRParser::parse(std::istream& a_input_stream, bool a_encoded)
{
    bool ended_with_error = false;
    std::string error_message;

    {
    // musimy ustawić tak locale, by kropka oddzielała część dziesiętną
    char *saved_locale = setlocale(LC_NUMERIC, NULL);
    if (saved_locale) {
        saved_locale = strdup(saved_locale);
        setlocale(LC_NUMERIC, "C");
    }

    grlexlineno = 0;
    grparser_all = 0;

    first_read = true;
    encoded = a_encoded;

    grlex_input_stream = &a_input_stream;
    if (grparserparse())
    {
        ended_with_error = true;
        error_message = grerror_message;
    }

    //przywracamy stare locale
    if (saved_locale) {
        setlocale(LC_NUMERIC, saved_locale);
        free(saved_locale);
    }
    }

    if (ended_with_error)
    throw ParseError(0, error_message);

    return grparser_all;
}
