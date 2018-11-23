/**
 * @file figure.cc
 */

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "builtin.h"
#include "env.h"
#include "exp.h"
#include "type.h"

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
    std::stringstream input_buffer;
    input_buffer << input_file.rdbuf();
    std::string input = input_buffer.str();

    // Parse input.
    Exp exp(input, 0, input.length());

    // Print expression.
    exp.print();
    std::cout << std::endl;

    // Build initial environment.
    Env init_env;
    init_env.builtin("let", builtin_let);
    init_env.builtin("print", builtin_print);

    // Evaluate expression and print result.
    Exp* result = exp.eval(init_env);
    result->print();
    std::cout << std::endl;

    return 0;
}
