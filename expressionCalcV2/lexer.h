#pragma once
#include <vector>
#include <string>
#include <exception>
#include <stdexcept>
#include <math.h>
#include <iostream>
#include <regex>
#include <algorithm>

enum class tokenType_t {
	NONE = 0,
	NUMBER, 
	PLUS,
	MINUS,
	DIVIDE,
	MULTIPLY,
	BRAKET_OPEN,
	BRAKET_CLOSE
};



class token_t {
public:
	token_t() {}
	token_t(std::string token) { tokenString = token; }
	token_t& setToken(std::vector<std::string> tokens);
	token_t& setToken(std::string token) { tokenString = token; return *this; }
	token_t& evalToken();
	token_t& addMinus();
	std::string ToString() { return tokenString; }
	std::string getTokenStr() { return tokenString; }
	long double getTokenNum();
	tokenType_t getTokenType() { return tokenType; }
	std::string tokenString = "";
	inline bool operator==(const token_t& other) const { return this->tokenString == other.tokenString; }

private:
	tokenType_t getTokenType(std::string str);
	tokenType_t tokenType = tokenType_t::NONE;
	long double number = 0;
};

class lexer_t {
public:
	lexer_t& setTokens(std::vector<std::string> tokens) { _tokenStrList = tokens; return *this; };
	lexer_t& setExpression(std::string str) { _expressionStr = str; return *this; }
	std::vector<token_t> getTokens() { return _tokenList; }
	lexer_t& parseAndEval();
	lexer_t& parseToken();
	lexer_t& evalToken();
	lexer_t& evalNegatives();
	std::vector<std::string> getStrList() {return _tokenStrList; }
	lexer_t& clear();
private:
	std::string _expressionStr = "";
	std::vector<std::string> _tokenStrList = {};
	std::vector<token_t> _tokenList = {};
};


