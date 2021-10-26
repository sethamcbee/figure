/**
 * @file literal.h
 */

#pragma once

#include <string>

namespace Figure
{

struct Bool
{
    bool value;
};

struct Number
{
    double value;
};

struct Char
{
    char value;
};

struct String
{
    std::string value;
};

struct Id
{
    std::string value;
};

}
