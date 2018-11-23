/**
 * @file builtin.h
 * @brief Interpreter built-in forms.
 */

#ifndef BUILTIN_H
#define BUILTIN_H

#include <vector>

class Exp;
class Env;

Exp* builtin_let(Env& env, std::vector<Exp*>& args);
Exp* builtin_if(Env& env, std::vector<Exp*>& args);
Exp* builtin_print(Env& env, std::vector<Exp*>& args);
Exp* builtin_add(Env& env, std::vector<Exp*>& args);

#endif // BUILTIN_H
