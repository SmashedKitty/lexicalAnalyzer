#pragma once
#include <string>

//Tokens: KEYWORD, ID, CONST, OPERATOR, DELIMITER, INVALID
class FiniteAutomats
{
	bool isKeyword(const std::string& lexeme);
	bool isIdentifier(const std::string& lexeme);
	bool isInteger(const std::string& lexeme);
	bool isOperator(const std::string& lexeme);
	bool isDelimiter(const std::string& lexeme);
	friend class Token;
};