#pragma once
#include <string>
#include <vector>
class Token
{
	friend class FiniteAutomats;
private:
	std::string lexeme;
	std::string type;//KEYWORD, ID, CONST, OPERATOR, DELIMITER, INVALID
public:
	Token(std::string lexeme);
	std::string getType() const;
	std::string getLexe() const;
};