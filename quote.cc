/**
 * @file quote.cc
 */

#include <quote.h>

namespace Figure
{

Quote::Quote(const DatumList& l)
{
    // Discard keyword.
    auto kwd = l.begin();
    auto datum = ++kwd;
    quoted = *datum;
}

void Quote::print() const
{
    std::cout << "'";
    quoted.print();
}

Ref<Exp> make_quote(const DatumList& l)
{
    Quote tmp{l};
    return make_ref(tmp);
}

const Datum& get_quoted(const Ref<Exp>& ref)
{
    auto p = std::static_pointer_cast<Quote>(ref);
    return p->quoted;
}

}
