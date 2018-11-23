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
    std::cout << "\n\n";

    // Build initial environment.
    Env init_env;
    Exp exp_true;
    exp_true.type = Type::BOOLEAN;
    exp_true.data = new bool(true);
    init_env.let("true", exp_true);
    Exp exp_false;
    exp_false.type = Type::BOOLEAN;
    exp_false.data = new bool(false);
    init_env.let("false", exp_false);
    init_env.builtin("if", builtin_if);
    init_env.builtin("let", builtin_let);
    init_env.builtin("print", builtin_print);
    init_env.builtin("+", builtin_add);

    // Evaluate expression and print result.
    Exp* result = exp.eval(init_env);
    result->print();
    std::cout << std::endl;

    return 0;
}
