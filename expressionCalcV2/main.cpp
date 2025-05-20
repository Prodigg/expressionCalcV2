#include "expressionCalc.h"
#include <iostream>
int main() {
	expressionCalc_t expressionCalc;
	long double result = 0;
	std::string userInput = "";
	std::cout << "Welcome to the Expression Calcolator v2.\n";
	while (true) {
		std::cout << "\nEnter a Expresion to Evaluate: ";
		std::getline(std::cin, userInput);
		result = expressionCalc.calculate(userInput);

		if (expressionCalc.getLastErrorPosition() != VALID_EXPRESSION)
			std::cout << "\nThis expression contains an error:\n" << expressionCalc.getLastErrorMsg() << "\n At token: " << expressionCalc.getLastErrorPosition() << "\n";
		else
			std::cout << "The result is: " << result << "\n";

	}
	return 0;
}