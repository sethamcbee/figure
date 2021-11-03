/**
 * @file quote.h
 */

#pragma once

namespace Figure
{

struct Datum;

struct Quote
{
    Datum quoted;

    void print() const;
};

}
