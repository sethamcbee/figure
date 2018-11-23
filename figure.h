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
    static std::shared_ptr<Exp> eval(const std::string& prog);
};

#endif // FIGURE_H
