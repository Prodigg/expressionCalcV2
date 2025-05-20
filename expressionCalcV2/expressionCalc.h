#pragma once
#include "evaluator.h"
#include "validator.h"

class expressionCalc_t {
public:
	expressionCalc_t() : _evaluator(_lexer) {		}
	double long calculate(std::string expression);
	std::string inline getLastErrorMsg() { return _validator.lastErrorMsg(); }
	int inline  getLastErrorPosition() { return _lastErrorPositon; }
private:
	int _lastErrorPositon = -1;
	lexer_t _lexer;
	validator_t _validator;
	evaluator_t _evaluator;
};

