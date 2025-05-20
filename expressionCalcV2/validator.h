#pragma once
#include "lexer.h"

#define VALID_EXPRESSION (int)-1

class validator_t {
public:
	validator_t& setAllowedTokenChar(std::vector<std::string> allowedStrings) { _allowedStrings = allowedStrings; return *this; }

	int validateEvaluator(std::vector<token_t> tokens);
	int validateParentheses(std::vector<token_t> tokens);
	int validateOperators(std::vector<token_t> tokens);

	int valdateLexer(std::string expression);
	int validateTokensString(std::string expression);

	std::string lastErrorMsg() { return _errorMsg; }

private:
	std::string _errorMsg = "";
	bool isOpperator(tokenType_t tokenType);
	std::vector<std::string> _allowedStrings = {"+", "-", "*", "/", "(", ")"};
};
