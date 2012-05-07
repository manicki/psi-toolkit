#ifndef EDGE_DESCRIPTION_HDR
#define EDGE_DESCRIPTION_HDR


#include <string>
#include <vector>


struct EdgeDescription {
    int id;
    int start;
    int end;
    std::string label;
    std::vector<int> children;

    EdgeDescription(
        int a_id,
        int a_start,
        int a_end,
        std::string a_label,
        std::vector<int> a_children
    ) :
        id(a_id),
        start(a_start),
        end(a_end),
        label(a_label),
        children(a_children)
    { }
};


#endif
