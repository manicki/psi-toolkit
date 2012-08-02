#include "grule_helpers.hpp"

#include <assert.h>

size_t grule_seq_length(const GRuleRightSymbolsSeq* seq)
{
    assert(seq);

    size_t s = 0;

    for (size_t i = 0; i < seq->size(); ++i)
    s += grule_alt_length((*seq)[i]);

    return s;
}

size_t grule_alt_length(const GRuleRightSymbolAltsSeq* alt)
{
    assert(alt);

    if (alt->atom)
    return 1;

    size_t m = 0;

    for (size_t i = 0; i < alt->alts.size(); ++i)
    {
    const size_t alen = grule_seq_length((alt->alts)[i]);
    if (alen > m)
        m = alen;
    }

    return m;
}
