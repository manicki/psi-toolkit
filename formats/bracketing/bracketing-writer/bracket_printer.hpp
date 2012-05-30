#ifndef BRACKET_PRINTER_HDR
#define BRACKET_PRINTER_HDR


#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <boost/foreach.hpp>

#include "edge_data.hpp"


class BracketPrinter {

public:

    BracketPrinter(
        std::vector<std::string> patterns,
        std::string tagSeparator,
        std::string avPairsSeparator,
        std::string avSeparator
    ) :
        patterns_(patterns),
        tagSeparator_(tagSeparator),
        avPairsSeparator_(avPairsSeparator),
        avSeparator_(avSeparator)
    { }

    std::set< std::vector<std::string> > print(
        std::set<EdgeData> edgeDataSet
    );

private:

    std::vector<std::string> patterns_;
    std::string tagSeparator_;
    std::string avPairsSeparator_;
    std::string avSeparator_;

    char close_(char c);

};


#endif
