#include "lexer.h"

token_t& token_t::evalToken() {

	try {
		number = std::stold(tokenString);
	} catch (const std::exception&) {
		number = std::nanl("");
	}
	
	
	if (!isnan(number)) {
		tokenType = tokenType_t::NUMBER;
		return *this;
	}
	
	tokenType = getTokenType(tokenString);
	if (tokenType == tokenType_t::NONE) throw std::logic_error("token is invalid");

	return *this;
}

token_t& token_t::setToken(std::vector<std::string> tokens) {
	tokenString = "";
	for (std::string& str : tokens) {
		tokenString += str;
	}
	return *this;
}

long double token_t::getTokenNum() {
	if (tokenType != tokenType_t::NUMBER) throw std::logic_error("Token isnt a number");
	return number;
}

token_t& token_t::addMinus() {
	if (tokenString.at(0) == '-') throw std::invalid_argument("number already negative");
	tokenString.insert(tokenString.begin(), '-');
	return evalToken();
}

tokenType_t token_t::getTokenType(std::string str) {
	if (str == "+") return tokenType_t::PLUS;
	if (str == "-") return tokenType_t::MINUS;
	if (str == "*") return tokenType_t::MULTIPLY;
	if (str == "/") return tokenType_t::DIVIDE;
	if (str == "(") return tokenType_t::BRAKET_OPEN;
	if (str == ")") return tokenType_t::BRAKET_CLOSE;
	return tokenType_t::NONE;
}


lexer_t& lexer_t::parseToken() {
	std::string workingStr = _expressionStr;
	workingStr.erase(std::remove_if(workingStr.begin(), workingStr.end(), isspace), workingStr.end());

	std::regex numWithDecimal("^[0-9]+\\.{0,1}[0-9]+");
	std::regex numWithoutDecimal("^[0-9]+");
	std::smatch match;

	while (workingStr != "") {
		if (std::regex_search(workingStr, match, numWithDecimal)) {
			_tokenStrList.push_back(workingStr.substr(0, match.length()));
			workingStr.erase(0, match.length());
			continue;
		}

		if (std::regex_search(workingStr, match, numWithoutDecimal)) {
			_tokenStrList.push_back(workingStr.substr(0, match.length()));
			workingStr.erase(0, match.length());
			continue;
		}
		_tokenStrList.push_back(workingStr.substr(0, 1));
		workingStr.erase(0, 1);
	}

	return *this;
}

lexer_t& lexer_t::evalToken() {
	for (std::string& str : _tokenStrList) {
		_tokenList.push_back(token_t(str));
	}

	for (token_t& token : _tokenList) {
		token.evalToken();
	}

	return *this;
}

lexer_t& lexer_t::clear() {
	_expressionStr = "";
	_tokenList = {};
	_tokenStrList = {};
	return *this;
}

lexer_t& lexer_t::parseAndEval() {
	return parseToken().evalToken().evalNegatives();
}

lexer_t& lexer_t::evalNegatives() {
	bool allNegativesOk = false;
	while (!allNegativesOk) {
		allNegativesOk = true;
		for (size_t i = 0; i < _tokenList.size(); i++) {
			if (_tokenList.at(i).getTokenType() == tokenType_t::MINUS && i == 0 && _tokenList.at(i + 1).getTokenType() == tokenType_t::NUMBER) {
				_tokenList.at(i + 1).addMinus();
				allNegativesOk = false;
				_tokenList.erase(_tokenList.begin() + i);
			}
			if (_tokenList.at(i).getTokenType() == tokenType_t::MINUS && _tokenList.at(i - 1).getTokenType() != tokenType_t::NUMBER && _tokenList.at(i + 1).getTokenType() == tokenType_t::NUMBER) {
				_tokenList.at(i + 1).addMinus();
				allNegativesOk = false;
				_tokenList.erase(_tokenList.begin() + i);
			}
		}
	}
	return *this;
}