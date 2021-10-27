/**
 * @file main.cc
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "figure.h"

int main(int argc, char* argv[])
{
	// Check if this is interactive.
	if (argc == 1)
	{
		Figure::Program program(std::cin);
		program.print();
		std::cout << "\nEval:\n======\n";
		program.eval();
		return 0;
	}
	
    // Verify that an input and output file were provided.
    if (argc != 2 && argc != 3)
    {
        std::cerr << "Invalid parameters.\n";
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

    // Parse and evaluate.
    Figure::Program program(input_file);
    program.print();
    std::cout << "\nEval:\n======\n";
    program.eval();

    // Compile if an output file was specified.
    if (argc == 3)
    {
        std::string output_filename{argv[2]};
        program.compile(output_filename);
    }

    return 0;
}
