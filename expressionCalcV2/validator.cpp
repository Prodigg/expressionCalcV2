#include "validator.h"

int validator_t::valdateLexer(std::string expression) {
	return validateTokensString(expression);
}

int validator_t::validateTokensString(std::string expression) {
	lexer_t validationLexer;
	validationLexer.clear().setExpression(expression).parseToken();
	std::vector<std::string> stringList = validationLexer.getStrList();

	for (size_t i = 0; i < stringList.size(); i++) {
		std::string& token = stringList.at(i);
		if (token.size() == 0) throw std::logic_error("empty string as token. Could indecate a problem with the lexer.");

		// check for valid char
		bool validStringFound = false;
		if (token.size() == 1) {
			for (std::string& c : _allowedStrings) {
				if (token == c) {
					validStringFound = true;
					break;
				}
			}
		}
		if (validStringFound) continue;

		// check for valid number
		try {
			long double num = std::stold(token);
			continue;
		}
		catch (const std::exception&) { }
		
		_errorMsg = "unknown character found.";
		return (int)i;
	}
	return VALID_EXPRESSION;
}

int validator_t::validateEvaluator(std::vector<token_t> tokens) {
	int result = VALID_EXPRESSION;
	result = validateOperators(tokens);
	if (result != VALID_EXPRESSION) return result;

	return validateOperators(tokens);
}

int validator_t::validateParentheses(std::vector<token_t> tokens) {
	int paratethiesOpenCount = 0;
	for (size_t i = 0; i < tokens.size(); i++) {
		token_t& token = tokens.at(i);
		if (token.getTokenType() == tokenType_t::BRAKET_OPEN) paratethiesOpenCount++;
		if (token.getTokenType() == tokenType_t::BRAKET_CLOSE) paratethiesOpenCount--;
		if (paratethiesOpenCount < 0) { 
			_errorMsg = "closing parenthese not expected.";
			return (int)i; 
		}
	}
	if (paratethiesOpenCount != 0) { 
		_errorMsg = "closing parenthese expected, not found.";
		return (int)tokens.size(); 
	}
	return VALID_EXPRESSION;
}

int validator_t::validateOperators(std::vector<token_t> tokens) {
	if (isOpperator(tokens.at(0).getTokenType())) {
		_errorMsg = "opperator not allowed at beginning of expression.";
		return 0;
	}

	if (isOpperator(tokens.at(tokens.size() - 1).getTokenType())) {
		_errorMsg = "opperator not allowed at end of expression.";
		return tokens.size() - 1;
	}

	for (size_t i = 1; i < tokens.size() - 1; i++) {
		token_t& token = tokens.at(i);

		if (isOpperator(token.getTokenType())) {
			if (isOpperator(tokens.at(i - 1).getTokenType()) || isOpperator(tokens.at(i + 1).getTokenType())) {
				_errorMsg = "unexpected opperator.";
				return i;
			}
		}

		if (token.getTokenType() == tokenType_t::NUMBER) {
			if (tokens.at(i - 1).getTokenType() == tokenType_t::NUMBER || tokens.at(i + 1).getTokenType() == tokenType_t::NUMBER) {
				_errorMsg = "unexpected number.";
				return i;
			}
		}

		if (token.getTokenType() == tokenType_t::BRAKET_OPEN) {
			if (!isOpperator(tokens.at(i - 1).getTokenType()) || tokens.at(i + 1).getTokenType() != tokenType_t::NUMBER) {
				_errorMsg = "unexpected parenthese.";
				return i;
			}
		}

		if (token.getTokenType() == tokenType_t::BRAKET_CLOSE) {
			if (!isOpperator(tokens.at(i + 1).getTokenType()) || tokens.at(i - 1).getTokenType() != tokenType_t::NUMBER) {
				_errorMsg = "unexpected parenthese.";
				return i;
			}
		}
	}

	_errorMsg = "";
	return VALID_EXPRESSION;
}

bool validator_t::isOpperator(tokenType_t tokenType) {
	switch (tokenType) {
	case tokenType_t::NONE:
		throw std::logic_error("invalid token type");
	case tokenType_t::NUMBER:
		return false;
	case tokenType_t::PLUS:
		return true;
	case tokenType_t::MINUS:
		return true;
	case tokenType_t::DIVIDE:
		return true;
	case tokenType_t::MULTIPLY:
		return true;
	case tokenType_t::BRAKET_OPEN:
		return false;
	case tokenType_t::BRAKET_CLOSE:
		return false;
	default:
		throw std::logic_error("unreachable");
	}
}