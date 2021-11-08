/**
 * @file datum.h
 */

#pragma once

#include <iostream>
#include <list>
#include <string_view>
#include <variant>
#include <vector>

#include "literal.h"

namespace Figure
{

struct Datum;
struct Sexp;
struct DatumPair;

using DatumList = std::list<Datum>;
using DatumVector = std::vector<Datum>;

struct DatumPair
{
    DatumList value;

    operator DatumList&();
    operator DatumList() const;
};

struct Datum
{
    using Value = std::variant<Bool, Char, Number, String, Id,
          DatumList, DatumVector, DatumPair>;

    Value value;
    Location pos;
    const Sexp* src = nullptr;

    Datum();
    Datum(Value v, Location p, const Sexp* s);
    Datum(const Sexp& s);

    void print(std::ostream& o) const;

    void error();
    void error(std::string_view e);
};

}
