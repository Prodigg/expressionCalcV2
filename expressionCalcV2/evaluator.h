#pragma once
#include "lexer.h"

class evaluator_t {
public:
	evaluator_t(lexer_t& lexer) : _lexer(lexer) {}
	long double evaluate();
private:
	long double evaluateTokens(std::vector<token_t>& tokens);
	bool paratethiesRemovable(std::vector<token_t>& tokens);
	int findLeastSignificantOpp(std::vector<token_t>& tokens);
	lexer_t& _lexer;
};
