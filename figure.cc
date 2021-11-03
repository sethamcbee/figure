/**
 * @file figure.cc
 */

#include "figure.h"
#include "lexer.h"

Figure::Program::Program(std::istream& input)
{
    env = init_env();
    ctx = &env;

    lexer = Figure::Lexer(input);
    while (lexer.good())
    {
        auto sexp = Sexp{lexer};
        sexps.push_back(sexp);
    }

    std::cout << "=====\nsexp:\n=====\n";
    for (const auto& sexp : sexps)
    {
        sexp.print();
        std::cout << std::endl;
    }

    std::cout << "=====\ndata:\n=====\n";
    for (const auto& sexp : sexps)
    {
        auto datum = Datum{sexp};
        datum.print();
        std::cout << std::endl;
        data.push_back(datum);
    }

    std::cout << "=====\nexp:\n=====\n";
    for (const auto & datum : data)
    {
        auto exp = Exp{ctx, datum};
        exp.print();
        std::cout << std::endl;
        exps.push_back(exp);
        ctx = &exps.back().env;
    }
}
