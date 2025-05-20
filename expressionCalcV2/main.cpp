#include "lexer.h"
#include "evaluator.h"
#include "validator.h"
int main() {
	validator_t testValidator;
	lexer_t lexer;
	lexer.setExpression(")(-2.0 + 5.0) * -3.0").parseAndEval();
	testValidator.validateParentheses(lexer.getTokens());
}