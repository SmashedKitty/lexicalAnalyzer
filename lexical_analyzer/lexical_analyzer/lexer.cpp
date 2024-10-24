#include "lexer.h"
#include <iostream>
#include <fstream>
void lexer::printHfile(std::string& pathOut)
{
	hashT.printFOut(pathOut);
}

void lexer::printH()
{
	hashT.print();
}

void lexer::IniTHashTable()
{
	for (int i = 0; i < Tokens.size(); ++i)
	{
		hashT.add(Tokens[i]);
	}
}

void lexer::InitTokens()
{
	std::string x;
	while (!this->isEnd)
	{
		x = nextLexeme(this->path, this->isOpen, this->isEnd, this->file);
		if (x != "")
		{
			Token tok(x);
			Tokens.push_back(tok);
		}
	}

}


lexer::lexer(const std::string& inFile)
{
	this->isOpen = false;
	this->isEnd = false;
	this->path = inFile;

	InitTokens();
	IniTHashTable();
}


std::string lexer::nextLexeme(std::string& filePath, bool& isFileOpen, bool& isEnd, std::ifstream& file) {
	std::string lexeme = "";
	char ch;

	// Open the file on the first call
	if (!isFileOpen) {
		file.open(filePath);
		if (!file.is_open()) {
			throw std::runtime_error("Could not open file");
		}
		isFileOpen = true;
	}
	if (file.eof())
	{
		isEnd = true;
		return "";
	}

	file.get(ch);
	while (ch != ' ' and ch != '\t' and ch != '\n' and !file.eof())
	{
		lexeme += ch;
		file.get(ch);
	}

	if (lexeme != "" and isEnd != true)
	{
		return lexeme;
	}
	else
		return nextLexeme(filePath, isFileOpen, isEnd, file);
}