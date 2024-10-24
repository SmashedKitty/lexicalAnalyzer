#include "Token.h"
#include "FiniteAutomats.h"

Token::Token(std::string lexeme)
{
	this->lexeme = lexeme;

	FiniteAutomats fa;

	bool f = 1;

	try {
		if (fa.isDelimiter(lexeme)) {
			this->type = "DELIMITER";
			f = 0;
		}
	}
	catch (const std::exception&) {}

	try {
		if (fa.isOperator(lexeme)) {
			this->type = "OPERATOR";
			f = 0;
		}
	}
	catch (const std::exception&) {}

	try {
		if (fa.isInteger(lexeme)) {
			this->type = "CONST";
			f = 0;
		}
	}
	catch (const std::exception&) {}

	try {
		if (fa.isIdentifier(lexeme)) {
			this->type = "IDENTIFIER";
			f = 0;
		}
	}
	catch (const std::exception&) {}

	try {
		if (fa.isKeyword(lexeme)) {
			this->type = "KEYWORD";
			f = 0;
		}
	}

	catch (const std::exception&) {}

	if (f) {
		this->type = "INVALID";
	}
}

std::string Token::getLexe() const
{
	return lexeme;
}
std::string Token::getType() const
{
	return type;
}

