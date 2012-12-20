#ifndef CHOOSER_HPP_HDR
#define CHOOSER_HPP_HDR

#include <vector>

//template<class Ch, class K>
//class chooser
//{
//public:
//    virtual std::vector<typename Ch::edge_descriptor> go(
//  Ch& chart, K& combinator,
//  const std::vector<typename K::rule_holder>& local_rules) = 0;
//};

template<class Ch, class K>
class longest_left_to_right_chooser //: public chooser<Ch,K>
{
public:
  typedef typename K::rule_holder rule_holder;
   std::vector<typename Ch::edge_descriptor> go(Ch& chart, K& combinator,const std::vector<typename longest_left_to_right_chooser<Ch,K>::rule_holder>& local_rules);
};

#endif
