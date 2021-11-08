/**
 * @file figure.cc
 */

#include "command.h"
#include "definition.h"
#include "figure.h"
#include "lexer.h"

namespace Figure
{

Program::Program(std::istream& input)
{
    env = init_env();
    ctx = &env;
    lexer = Lexer(input);
    while (lexer.good())
    {
        auto sexp = Sexp{lexer};
        sexps.push_back(sexp);
    }
    for (const auto& sexp : sexps)
    {
        auto datum = Datum{sexp};
        data.push_back(datum);
    }
    for (const auto& datum : data)
    {
        Ref<Exp> exp;
        if (is_definition(datum))
        {
            exp = make_definition(env, datum);
        }
        else
        {
            exp = make_command(env, datum);
        }
        exps.push_back(exp);
    }
}

void Program::print()
{
    for (const auto& exp : exps)
    {
        exp->print(std::cout);
        std::cout << std::endl;
    }
}

void Program::eval()
{
    for (const auto& exp : exps)
    {
        auto ret = exp->eval(env);
        if (ret)
        {
            ret->print(std::cout);
        }
    }
}

void Program::repl()
{
    auto env = init_env();
    auto lexer = Lexer{std::cin};
    auto sexps = std::list<Sexp> {};
    auto data = std::list<Datum> {};
    auto exps = std::list<Ref<Exp>> {};
    while (lexer.good())
    {
        auto sexp = Sexp{lexer};
        sexps.push_back(sexp);
        auto datum = Datum{sexp};
        data.push_back(datum);
        Ref<Exp> exp;
        if (is_definition(datum))
        {
            exp = make_definition(env, datum);
        }
        else
        {
            exp = make_command(env, datum);
        }
        exps.push_back(exp);
        exp->print(std::cout);
        std::cout << std::endl;
        auto ret = exp->eval(env);
        if (ret)
        {
            ret->print(std::cout);
        }
    }
}

void Program::compile(std::string_view output)
{
    // Stub.
}

}
