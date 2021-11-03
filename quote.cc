/**
 * @file quote.cc
 */

#include <iostream>

#include "datum.h"
#include "quote.h"

namespace Figure
{

void Quote::print() const
{
    std::cout << "'";
    quoted.print();
}

}
