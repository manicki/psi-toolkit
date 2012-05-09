#ifndef EDGE_DESCRIPTION_HDR
#define EDGE_DESCRIPTION_HDR


#include <list>
#include <string>


struct EdgeDescription {
    int start;
    int end;
    std::string label;
    std::list<int> children;
    std::list<int> rsiblings;

    EdgeDescription(
        int a_start,
        int a_end,
        std::string a_label,
        std::list<int> a_children,
        std::list<int> a_rsiblings
    ) :
        start(a_start),
        end(a_end),
        label(a_label),
        children(a_children),
        rsiblings(a_rsiblings)
    { }
};


#endif
