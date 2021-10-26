/**
 * @file env.h
 */

#pragma once

#include <map>
#include <utility>

#include "sexp.h"
#include "exp.h"

namespace Figure
{

using EnvMap = std::map<Id, Exp>;

class Env
{
public:

    Exp* get(const Id& i);

    void error();
    void error(std::string_view msg);

private:

    Env* parent;
    EnvMap data;
};

};
