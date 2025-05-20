#include "pch.h"
#include "CppUnitTest.h"

#include "..\expressionCalcV2\lexer.h"
#include "..\expressionCalcV2\lexer.cpp"

#include "..\expressionCalcV2\evaluator.h"
#include "..\expressionCalcV2\evaluator.cpp"

#include "..\expressionCalcV2\validator.h"
#include "..\expressionCalcV2\validator.cpp"

#include "..\expressionCalcV2\expressionCalc.h"
#include "..\expressionCalcV2\expressionCalc.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework {

			template <>
			inline std::wstring ToString(const std::vector<token_t>& v) {
				std::wostringstream oss;
				oss << L"[";
				for (size_t i = 0; i < v.size(); ++i) {
					oss << v.at(i).tokenString.c_str();
					if (i < v.size() - 1) {
						oss << L", ";
					}
				}
				oss << L"]";
				return oss.str();
			}
		}
	}
}

namespace UnitTestExpressionCalc
{
	TEST_CLASS(tokenUnitTest)
	{
	public:
		
		TEST_METHOD(tokenNumber) {
			token_t testToken;
			double num = testToken.setToken("-10.8").evalToken().getTokenNum();

			Assert::AreEqual(-10.8, num);
			Assert::IsTrue((tokenType_t::NUMBER == testToken.getTokenType()));

			num = testToken.setToken({ "-","10",".","8" }).evalToken().getTokenNum();

			Assert::AreEqual(-10.8, num);
			Assert::IsTrue((tokenType_t::NUMBER == testToken.getTokenType()));
		}

		TEST_METHOD(otherToken) {
			token_t testToken;
			Assert::IsTrue((tokenType_t::PLUS == testToken.setToken("+").evalToken().getTokenType()));
			Assert::IsTrue((tokenType_t::MINUS == testToken.setToken("-").evalToken().getTokenType()));
			Assert::IsTrue((tokenType_t::MULTIPLY == testToken.setToken("*").evalToken().getTokenType()));
			Assert::IsTrue((tokenType_t::DIVIDE == testToken.setToken("/").evalToken().getTokenType()));
			Assert::IsTrue((tokenType_t::BRAKET_OPEN == testToken.setToken("(").evalToken().getTokenType()));
			Assert::IsTrue((tokenType_t::BRAKET_CLOSE == testToken.setToken(")").evalToken().getTokenType()));
		}

		TEST_METHOD(invalidToken) {
			Assert::ExpectException<std::logic_error>([=](void) -> void {
				token_t testToken;
				testToken.setToken("invalid token").evalToken();
			});

			Assert::ExpectException<std::logic_error>([=](void) -> void {
				token_t testToken;
				testToken.setToken("(").evalToken().getTokenNum();
			});
		}
	};

	TEST_CLASS(lexerUnitTest) {
	public:
		TEST_METHOD(parseExpression) {
			lexer_t testLexer;
			lexer_t expectedLexer;

			expectedLexer.setTokens({ "1", "+", "2" }).parseToken().evalToken();
			testLexer.setExpression("1 + 2").parseToken().evalToken();
			Assert::AreEqual<std::vector<token_t>>(expectedLexer.getTokens(), testLexer.getTokens());

			testLexer.clear();
			expectedLexer.clear();

			expectedLexer.setTokens({ "-", "7", "/", "(", "-", "2", ")" }).parseToken().evalToken();
			testLexer.setExpression("- 7 / (-2)").parseToken().evalToken();
			Assert::AreEqual<std::vector<token_t>>(expectedLexer.getTokens(), testLexer.getTokens());
		}

		TEST_METHOD(invalidExpression) {
			Assert::ExpectException<std::logic_error>([=](void) -> void {
				lexer_t testLexer;
				testLexer.setExpression("invalid expression").parseToken().evalToken();
			});
		}
	};
	TEST_CLASS(evaluatorTest) {
		TEST_METHOD(evalTestSimpeExpressions) {
			lexer_t lexer;
			evaluator_t testEvaluator(lexer);
			lexer.setExpression("1 + 1").parseAndEval();
			Assert::AreEqual(2.0, (double)testEvaluator.evaluate());

			lexer.clear().setExpression("3.5 + 2.1").parseAndEval();
			Assert::AreEqual(5.6, (double)testEvaluator.evaluate());
			
			lexer.clear().setExpression("10.0 - 4.5").parseAndEval();
			Assert::AreEqual(5.5, (double)testEvaluator.evaluate());

			lexer.clear().setExpression("6.2 * 3.0").parseAndEval();
			Assert::AreEqual(18.6, (double)testEvaluator.evaluate());

			lexer.clear().setExpression("9.0 / 3.0").parseAndEval();
			Assert::AreEqual(3.0, (double)testEvaluator.evaluate());

		}

		TEST_METHOD(evalTestOperatorPrecedence) {
			lexer_t lexer;
			evaluator_t testEvaluator(lexer);
			lexer.setExpression("2.0 + 3.0 * 4.0").parseAndEval();
			Assert::AreEqual(14.0, (double)testEvaluator.evaluate());

			lexer.clear().setExpression("10.0 - 2.0 * 3.0").parseAndEval();
			Assert::AreEqual(4.0, (double)testEvaluator.evaluate());

			lexer.clear().setExpression("100.0 / 10.0 + 5.0").parseAndEval();
			Assert::AreEqual(15.0, (double)testEvaluator.evaluate());

			lexer.clear().setExpression("4.0 + 6.0 / 2.0").parseAndEval();
			Assert::AreEqual(7.0, (double)testEvaluator.evaluate());

		}

		TEST_METHOD(evalTestParentheses) {
			lexer_t lexer;
			evaluator_t testEvaluator(lexer);
			lexer.setExpression("(2.0 + 3.0) * 4.0").parseAndEval();
			Assert::AreEqual(20.0, (double)testEvaluator.evaluate());

			lexer.clear().setExpression("(10.0 - 2.0) / 2.0").parseAndEval();
			Assert::AreEqual(4.0, (double)testEvaluator.evaluate());

			lexer.clear().setExpression("(5.0 + 5.0) * (2.0 + 3.0)").parseAndEval();
			Assert::AreEqual(50.0, (double)testEvaluator.evaluate());

			lexer.clear().setExpression("((3.0 + 2.0) * (6.0 - 1.0)) / 5.0").parseAndEval();
			Assert::AreEqual(5.0, (double)testEvaluator.evaluate());

		}

		TEST_METHOD(evalTestEdgeCases) {
			lexer_t lexer;
			evaluator_t testEvaluator(lexer);
			lexer.setExpression("0.0 / 1.0").parseAndEval();
			Assert::AreEqual(0.0, (double)testEvaluator.evaluate());

			lexer.clear().setExpression("5.5 - (2.2 + 3.3)").parseAndEval();
			Assert::AreEqual(0.0, (double)testEvaluator.evaluate());

			lexer.clear().setExpression("(8.8 - 4.4) * 2.0 + 1.1").parseAndEval();
			Assert::AreEqual(9.9, (double)testEvaluator.evaluate());

			lexer.clear().setExpression("-0.0 + 0.0").parseAndEval();
			Assert::AreEqual(0.0, (double)testEvaluator.evaluate());

			lexer.clear().setExpression("0.0 - -3.3").parseAndEval();
			Assert::AreEqual(3.3, (double)testEvaluator.evaluate());

		}

		TEST_METHOD(evalTestNegativeNumbers) {
			lexer_t lexer;
			evaluator_t testEvaluator(lexer);
			lexer.setExpression("-5.0 + 3.0").parseAndEval();
			Assert::AreEqual(-2.0, (double)testEvaluator.evaluate());

			lexer.clear().setExpression("(-2.0 + 5.0) * -3.0").parseAndEval();
			Assert::AreEqual(-9.0, (double)testEvaluator.evaluate());

			lexer.clear().setExpression("-4.0 * (2.0 + 1.0)").parseAndEval();
			Assert::AreEqual(-12.0, (double)testEvaluator.evaluate());

			lexer.clear().setExpression("-3.0 - -6.0").parseAndEval();
			Assert::AreEqual(3.0, (double)testEvaluator.evaluate());

		}
	};

	TEST_CLASS(validatorUnitTest) {
		TEST_METHOD(lexerValidationTokenString) {
			validator_t testValidator;
			Assert::AreEqual(VALID_EXPRESSION, testValidator.validateTokensString("1 + 1 * 1 - 1 / 1"));
			Assert::AreEqual(VALID_EXPRESSION, testValidator.validateTokensString("10000"));
			Assert::AreEqual(VALID_EXPRESSION, testValidator.validateTokensString("-10.1"));
			Assert::AreEqual(VALID_EXPRESSION, testValidator.validateTokensString("10.9 + -19.0"));

			Assert::AreEqual(0, testValidator.validateTokensString("a10.9 + -19.0"));
			Assert::AreEqual(2, testValidator.validateTokensString("10.9 +a -19.0"));
			Assert::AreEqual(4, testValidator.validateTokensString("10.9 + -19.0$"));

		}

		TEST_METHOD(expressionValidationParentheses) {
			lexer_t lexer;
			validator_t testValidator;
			lexer.clear().setExpression("(-2.0 + 5.0) * -3.0").parseAndEval();
			Assert::AreEqual(VALID_EXPRESSION, testValidator.validateParentheses(lexer.getTokens()));

			lexer.clear().setExpression("5.5 - (2.2 + 3.3)").parseAndEval();
			Assert::AreEqual(VALID_EXPRESSION, testValidator.validateParentheses(lexer.getTokens()));

			lexer.clear().setExpression("((((((1))))))").parseAndEval();
			Assert::AreEqual(VALID_EXPRESSION, testValidator.validateParentheses(lexer.getTokens()));

			lexer.clear().setExpression(")(-2.0 + 5.0) * -3.0").parseAndEval();
			Assert::AreEqual(0, testValidator.validateParentheses(lexer.getTokens()));

			lexer.clear().setExpression("(-2.0 + 5.0 * -3.0").parseAndEval();
			Assert::AreEqual(6, testValidator.validateParentheses(lexer.getTokens()));
		}

		TEST_METHOD(expressionValidationOperators) {
			lexer_t lexer;
			validator_t testValidator;
			lexer.clear().setExpression("(-2.0 + 5.0) * -3.0").parseAndEval();
			Assert::AreEqual(VALID_EXPRESSION, testValidator.validateOperators(lexer.getTokens()));

			lexer.clear().setExpression("(-2.0 ++ 5.0) * -3.0").parseAndEval();
			Assert::AreEqual(2, testValidator.validateOperators(lexer.getTokens()));

			lexer.clear().setExpression("(-2.0 + 5.0) * -3.0").parseAndEval();
			Assert::AreEqual(VALID_EXPRESSION, testValidator.validateOperators(lexer.getTokens()));

			lexer.clear().setExpression("(-2.0( + 5.0)) * -3.0").parseAndEval();
			Assert::AreEqual(2, testValidator.validateOperators(lexer.getTokens()));

			lexer.clear().setExpression("+(-2.0 + 5.0) * -3.0").parseAndEval();
			Assert::AreEqual(0, testValidator.validateOperators(lexer.getTokens()));

			lexer.clear().setExpression("(-2.0( + 5.0)) -3.0").parseAndEval();
			Assert::AreEqual(2, testValidator.validateOperators(lexer.getTokens()));
			
		}
	};

	TEST_CLASS(expressionCalcUnitTest) {
		TEST_METHOD(validExpressions) {
			expressionCalc_t testExpressionCalc;
			Assert::AreEqual(2.0, (double)testExpressionCalc.calculate("1 + 1"));
			Assert::AreEqual(1.0, (double)testExpressionCalc.calculate("((((((1))))))"));
			Assert::AreEqual(2.0, (double)testExpressionCalc.calculate("1 + 1"));
			Assert::AreEqual(2.0, (double)testExpressionCalc.calculate("1 + 1"));
			Assert::AreEqual(2.0, (double)testExpressionCalc.calculate("1 + 1"));
		}
	};
}
