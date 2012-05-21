#ifndef ACTIVE_ELEMENTS_PRINTER_HDR
#define ACTIVE_ELEMENTS_PRINTER_HDR


#include <map>
#include <set>
#include <string>

#include "edge_data.hpp"


class ActiveElementsPrinter {

public:

    class Manager {
    public:
        Manager(
            std::string tagSeparator,
            std::string avPairsSeparator,
            std::string avSeparator
        ) :
            tagSeparator_(tagSeparator),
            avPairsSeparator_(avPairsSeparator),
            avSeparator_(avSeparator)
        { }

        ActiveElementsPrinter getPrinter(std::string pattern);

    private:
        std::string tagSeparator_;
        std::string avPairsSeparator_;
        std::string avSeparator_;
    };

    ActiveElementsPrinter(
        std::string pattern,
        std::string tagSeparator,
        std::string avPairsSeparator,
        std::string avSeparator
    ) :
        pattern_(pattern),
        tagSeparator_(tagSeparator),
        avPairsSeparator_(avPairsSeparator),
        avSeparator_(avSeparator)
    { }

    std::string print(
        EdgeData edgeData,
        std::string starText = ""
    );

    std::string print(
        std::set<std::string> tags,
        std::string category,
        std::string text,
        std::map<std::string, std::string> avMap,
        double score,
        std::string starText = ""
    );

private:

    std::string pattern_;
    std::string tagSeparator_;
    std::string avPairsSeparator_;
    std::string avSeparator_;

    char close_(char c);

};


#endif
