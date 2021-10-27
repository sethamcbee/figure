/**
 * @file figure.cc
 */

#include "figure.h"
#include "lexer.h"

Figure::Program::Program(std::istream& input)
{
    lexer = Figure::Lexer(input);
    while (lexer.good())
    {
        auto sexp = Sexp{lexer};
        sexps.push_back(sexp);
    }
    for (auto sexp : sexps)
    {
        sexp.print();
        std::cout << std::endl;
    }
}
