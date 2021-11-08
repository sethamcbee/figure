/**
 * @file quote.cc
 */

#include "quote.h"

namespace Figure
{

Quote::Quote(const Datum& datum)
{
    const auto& l = std::get<DatumList>(datum.value);

    // Discard keyword.
    auto kwd = l.begin();
    auto q = ++kwd;
    quoted = *q;
}

void Quote::print(std::ostream& o) const
{
    o << "'";
    quoted.print(o);
}

void Quote::error() const
{
    error("Unspecified error.");
}

void Quote::error(const std::string& err) const
{
    std::cerr << "At character: " << quoted.pos << std::endl;
    std::cerr << "At datum: ";
    quoted.print(std::cerr);
    std::cerr << std::endl;
    std::cerr << "Error parsing quote expression: ";
    std::cerr << err << std::endl;
}

Ref<Exp> make_quote(const Datum& datum)
{
    Quote tmp{datum};
    return make_ref(tmp);
}

const Datum& get_quoted(const Ref<Exp>& ref)
{
    auto p = std::static_pointer_cast<Quote>(ref);
    return p->quoted;
}

}
