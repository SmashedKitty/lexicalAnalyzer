#pragma once
#include "Token.h"
#include "FiniteAutomats.h"
#include "HashTable.h"
#include <vector>
#include <string>

class lexer
{
private:

	std::string path;
	std::ifstream file;
	std::ofstream fileOut;
	std::string pathOut;

	bool isOpen;
	bool isEnd;

	std::vector<Token> Tokens;
	FiniteAutomats automats;
	HashTable hashT;


	void InitTokens();
	void IniTHashTable();
	std::string nextLexeme(std::string& filePath, bool& isFileOpen, bool& isEnd, std::ifstream& file);

public:
	lexer(const std::string& inFile);
	void print()
	{
		for (int i = 0; i < this->Tokens.size(); ++i)
		{
			std::cout << "(" << Tokens[i].getLexe() << " , " << Tokens[i].getType() << ")" << std::endl;
		}
	}
	void printH();
	void printHfile(std::string& pathOut);

};
