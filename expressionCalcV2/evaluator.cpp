#include "evaluator.h"


long double evaluator_t::evaluate() {
	std::vector<token_t> tokenList = _lexer.getTokens();
	return evaluateTokens(tokenList);
}

int evaluator_t::findLeastSignificantOpp(std::vector<token_t>& tokens) {
	if (tokens.size() == 0) throw std::invalid_argument("no tokens provided");

	int retVal = -1;
	size_t bracetsOpen = 0;
	for (int i = 0; i < tokens.size(); i++) {
		switch (tokens.at(i).getTokenType()) {
		case tokenType_t::PLUS:
			if (bracetsOpen > 0) break;
			return i;
		case tokenType_t::MINUS:
			if (bracetsOpen > 0) break;
			return i;
		case tokenType_t::MULTIPLY:
			if (retVal > i) break;
			if (bracetsOpen > 0) break;
			retVal = i;
			break;
		case tokenType_t::DIVIDE:
			if (retVal > i) break;
			if (bracetsOpen > 0) break;
			retVal = i;
			break;
		case tokenType_t::BRAKET_OPEN:
			bracetsOpen++;
			break;
		case tokenType_t::BRAKET_CLOSE:
			bracetsOpen--;
			if (bracetsOpen < 0) {
				throw std::invalid_argument("closing brackets not valid");
			}
		default:
			// do nothing
			break;
		}
	}
	return retVal;
}

long double evaluator_t::evaluateTokens(std::vector<token_t>& tokens) {
	if (tokens.size() == 0) throw std::invalid_argument("empty list provided");
	if (tokens.size() == 1 && tokens.at(0).getTokenType() == tokenType_t::NUMBER) return tokens.at(0).getTokenNum();

	// remove brackets if needed
	if (paratethiesRemovable(tokens)) {
		tokens.erase(tokens.begin());
		tokens.erase(tokens.end() - 1);
	}

	if (tokens.size() == 0) throw std::invalid_argument("invalid brackets");

	//split tokens

	int index = findLeastSignificantOpp(tokens);

	if (index == -1) throw std::logic_error("no opperators given");


	// evaluate tokens
	std::vector<token_t> lhs {};
	std::vector<token_t> rhs {};
	
	token_t operatorToken = tokens.at(index);

	for (size_t i = 0; i < index; i++) {
		lhs.push_back(tokens.at(i));
	}

	for (size_t i = static_cast<size_t>(index) + 1; i < tokens.size(); i++) {
		rhs.push_back(tokens.at(i));
	}

	switch (operatorToken.getTokenType()) {
	case tokenType_t::PLUS:
		return evaluateTokens(lhs) + evaluateTokens(rhs);
	case tokenType_t::MINUS:
		return evaluateTokens(lhs) - evaluateTokens(rhs);
	case tokenType_t::MULTIPLY:
		return evaluateTokens(lhs) * evaluateTokens(rhs);
	case tokenType_t::DIVIDE:
		return evaluateTokens(lhs) / evaluateTokens(rhs);
	default:
		throw std::logic_error("invalid operator");
	}
	throw std::logic_error("unreachable");
}

bool evaluator_t::paratethiesRemovable(std::vector<token_t>& tokens) {
	if (!(tokens.at(0).getTokenType() == tokenType_t::BRAKET_OPEN && tokens.at(tokens.size() - 1).getTokenType() == tokenType_t::BRAKET_CLOSE))
		return false;

	int paratethiesOpenCount = 0;
	for (size_t i = 0; i < tokens.size(); i++) {
		token_t& token = tokens.at(i);
		if (token.getTokenType() == tokenType_t::BRAKET_OPEN) paratethiesOpenCount++;
		if (token.getTokenType() == tokenType_t::BRAKET_CLOSE) paratethiesOpenCount--;

		if (i != tokens.size() - 1 && paratethiesOpenCount == 0) return false;
		if (paratethiesOpenCount < 0) throw std::logic_error("invalid brackets");
	}
	return true;
}