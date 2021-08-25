// C++ program to evaluate a given expression
#include <iostream>
#include "parsing.h"
#include "shunting-yard.h"
#include "evaluating.h"

int main(int argc, char* argv[])
{
	std::string line; // Input line
	std::stack<std::string> output;  // Output for the Shunting-Yard algorithm

	getline(std::cin, line);  // Reading line and initialising Shunting-Yard

	// Parsing process
	ShuntingYard ShYard_line(line);

	try
	{
		ShYard_line.parsing();
	}
	catch (const std::invalid_argument& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 0;
	}

	// Evaluation process
	PostfixEvaluation pe;
	try
	{
		// AST Evaluation
		std::cout << pe.ast_evaluation(ShYard_line.get_ast()) << std::endl;
	}
	catch (const std::invalid_argument& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 0;
	}

	return 0;
}
