#ifndef DISASSEMBLE_EXPRESSION_HPP_HDR
#define DISASSEMBLE_EXPRESSION_HPP_HDR

#include <ostream>

template<class T, class S, int N, class M>
void disassemble_expression(
    const compiled_expression<T, S, N>& expr,
    const registrar<std::string>& attribute_registrar,
    const registrar<std::string>& extra_attribute_registrar,
    const M& master,
    std::ostream& s);



#endif
