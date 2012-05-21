#ifndef ACTIVE_ELEMENTS_PRINTER_HDR
#define ACTIVE_ELEMENTS_PRINTER_HDR


#include <map>
#include <set>
#include <string>


struct EdgeData {
    std::set<std::string> tags;
    std::string category;
    std::string text;
    std::map<std::string, std::string> avMap;
    double score;

    EdgeData(
        std::set<std::string> a_tags,
        std::string a_category,
        std::string a_text,
        std::map<std::string, std::string> a_avMap,
        double a_score
    ) :
        tags(a_tags),
        category(a_category),
        text(a_text),
        avMap(a_avMap),
        score(a_score)
    { }

    bool operator<(EdgeData other) const {
        return category < other.category ||
            text < other.text ||
            score < other.score ||
            tags < other.tags ||
            avMap < other.avMap;
    }
};


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
        EdgeData edgeData
    );

    std::string print(
        std::set<std::string> tags,
        std::string category,
        std::string text,
        std::map<std::string, std::string> avMap,
        double score
    );

private:

    std::string pattern_;
    std::string tagSeparator_;
    std::string avPairsSeparator_;
    std::string avSeparator_;

    char close_(char c);

};


#endif
