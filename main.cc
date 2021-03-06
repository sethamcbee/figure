/**
 * @file main.cc
 */

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "exp.h"
#include "figure.h"

int main(int argc, char** argv)
{
    // Verify that an input file was provided.
    if (argc <= 1)
    {
        std::cerr << "No input file specified.\n";
        return 1;
    }

    // Attempt to open the input file.
    const char* input_filename = argv[1];
    std::ifstream input_file;
    input_file.open(input_filename);

    // Verify that the input file is open.
    if (!input_file.is_open())
    {
        std::cerr << "Cannot open input file: " << input_filename << "\n";
        return 1;
    }

    // Read input file.
    std::stringstream input;
    input << input_file.rdbuf();
    input_file.close();

    // Print input file.
    std::cout << input.str() << std::endl;

    // Run program and print result.
    std::shared_ptr<Exp> result = Figure::run(input.str());
    std::cout << std::endl;
    result->print();
    std::cout << std::endl;

    return 0;
}
