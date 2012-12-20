#ifndef ACCOMMODATER_HDR_HPP
#define ACCOMMODATER_HDR_HPP

#include <map>

template<class H, class K>
class binary_accommodater
{
public:
    binary_accommodater(H& chart, K& combinator, size_t limit);

    void accommodate(typename H::edge_descriptor edge);

private:
    void accommodate_partition_(
    typename H::edge_descriptor edge,
    typename H::partition_iterator pit);

    void accommodate_unary_(
    typename H::edge_descriptor edge,
    typename H::partition_iterator pit);

    void accommodate_binary_(
    typename H::edge_descriptor edge,
    typename H::partition_iterator pit);

    H& chart_;
    K& combinator_;
    size_t limit_;

    std::map<typename H::edge_descriptor, bool> accommodated_;
};

#endif
