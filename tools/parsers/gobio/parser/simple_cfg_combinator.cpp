#include "simple_cfg_combinator.hpp"

#include <string>

template <>
int simple_cfg_rule<char>::last_rule_no_ = 0;

template <>
int simple_cfg_rule<std::string>::last_rule_no_ = 0;
