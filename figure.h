/**
 * @file figure.h
 */

#ifndef FIGURE_H
#define FIGURE_H

#include <memory>
#include <string>

class Exp;

class Figure
{
public:

    // Evaluate a program.
    static std::shared_ptr<Exp> run(const std::string& prog);

    // Evaluate a single top-level expression.
    static std::shared_ptr<Exp> eval(
        std::shared_ptr<Env> env, std::shared_ptr<Exp> exp);
};

#endif // FIGURE_H
