#include "expressionCalc.h"

double long expressionCalc_t::calculate(std::string expression) {
	_lastErrorPositon = _validator.valdateLexer(expression);
	if (_lastErrorPositon != VALID_EXPRESSION) return std::nanl("");

	_lexer.clear().setExpression(expression).parseAndEval();
	
	_lastErrorPositon = _validator.validateEvaluator(_lexer.getTokens());
	if (_lastErrorPositon != VALID_EXPRESSION) return std::nanl("");

	return _evaluator.evaluate();
}