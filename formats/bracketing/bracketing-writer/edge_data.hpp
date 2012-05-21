#ifndef EDGE_DATA_HDR
#define EDGE_DATA_HDR


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


#endif
