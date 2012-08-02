#include "compile_gruleexpression.tpl"

#include <stdexcept>


void right_symbols_to_hash(
    const GRuleRightSymbolsSeq& a_symbols_seq,
    HashWrapper<std::pair<int, int>, int>::type& right_symbols_hash,
    registrar<std::string>& symbol_registrar,
    int& current_ix,
    int& starred_ix)
{
    std::vector<GRuleRightSymbolAltsSeq*>::const_iterator iter =
    a_symbols_seq.begin();

    for ( ; iter != a_symbols_seq.end(); ++iter)
    right_symbol_alts_seq_to_hash(
        *(*iter),
        right_symbols_hash,
        symbol_registrar,
        current_ix,
        starred_ix);

    assert(current_ix > 0);
}

void right_symbol_alts_seq_to_hash(
    const GRuleRightSymbolAltsSeq& a_symbol_alts_seq,
    HashWrapper<std::pair<int, int>, int>::type& right_symbols_hash,
    registrar<std::string>& symbol_registrar,
    int& current_ix,
    int& starred_ix)
{
    if (a_symbol_alts_seq.atom)
    {
    assert(a_symbol_alts_seq.alts.empty());

    int symbol_id = symbol_registrar.get_id(*(a_symbol_alts_seq.atom));
    int symbol_no = 0;

    while (right_symbols_hash.count(
          std::pair<int, int>(
              symbol_id,
              symbol_no)))
        ++symbol_no;

    if (a_symbol_alts_seq.is_starred)
    {
        if (starred_ix == -1L)
        starred_ix = current_ix;
        else if (starred_ix != current_ix)
        throw std::runtime_error("symbols_wrongly_starred");
    }

    right_symbols_hash[std::pair<int, int>(symbol_id, symbol_no)]
        = current_ix++;
    }
    else
    {
    int subcurrent_ix;
    int max_ix = current_ix;

    std::vector<GRuleRightSymbolsSeq*>::const_iterator iter =
        a_symbol_alts_seq.alts.begin();

    for ( ; iter != a_symbol_alts_seq.alts.end() ; ++iter)
    {
        subcurrent_ix = current_ix;

        right_symbols_to_hash(
        *(*iter),
        right_symbols_hash,
        symbol_registrar,
        subcurrent_ix,
        starred_ix);

        if (subcurrent_ix > max_ix)
        max_ix = subcurrent_ix;
    }

    current_ix = max_ix;
    }
}


bool is_constant(const GRuleExpression* expr)
{
    assert(expr);

    return (expr->expr_type == GRuleExpression::NUMBER
        || expr->expr_type == GRuleExpression::ATOM);
}
