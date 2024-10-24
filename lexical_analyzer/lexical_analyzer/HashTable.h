#pragma once
#include <iostream>
#include <fstream>
#include "FiniteAutomats.h"
#include "Token.h"


class HashTable
{
private:
	struct Node
	{
		std::string type = "";
		std::string lexeme = "";
		int id = -1;

		bool empty()
		{
			if (this->lexeme == "")
				return true;
			else
				return false;
		}

		Node(std::string lexeme, int id)
		{
			Token tok(lexeme);
			this->type = tok.getType();
			this->id = id;
			this->lexeme = tok.getLexe();
		}

		Node(Token& tok, int id)
		{
			this->type = tok.getType();
			this->id = id;
			this->lexeme = tok.getLexe();
		}

		Node()
		{
			this->type = "";
			this->lexeme = "";
			this->id = -1;
		}
	};



	int size = 100;
	Node* Table = new Node[size];

	void resize(Node*& Table, int& Oldsize)
	{
		Node* new_ = new Node[Oldsize * 2];
		for (int i = 0; i < Oldsize; ++i)
		{
			new_[i] = Table[i];
		}
		this->Table = new_;
		this->size = Oldsize * 2;
	}

	int hash(std::string& lexeme)
	{
		int h = int(lexeme[0]);
		if (h > size)
		{
			resize(Table, size);
		}
		return h;
	}

	int rehash(int hash, int& i)
	{
		int h = hash * i;
		if (h > size)
		{
			resize(Table, size);
		}
		return h;
	}
public:
	HashTable()
	{
		int size = 100;
		Node* Table = new Node[size];

	}

	~HashTable()
	{
		delete[] Table;
	}

	void add(Token tok)
	{
		std::string l = tok.getLexe();
		std::string t = tok.getType();
		int h = hash(l);
		int i = 2;

		if (Table[h].empty())
		{
			Node A(tok, h);
			Table[h] = A;
		}
		else
		{
			int h1 = h;
			while (!Table[h1].empty())
			{
				h1 = rehash(h1, i);
			}
			Node A(tok, h1);
			Table[h] = A;
		}
	}

	void add(std::string lexeme)
	{
		int h = hash(lexeme);
		int i = 2;

		if (Table[h].empty())
		{
			Node A(lexeme, h);
			Table[h] = A;
		}
		else
		{
			int h1 = h;
			while (!Table[h1].empty())
			{
				h1 = rehash(h1, i);
			}
			Node A(lexeme, h1);
			Table[h] = A;
		}
	}

	void print()
	{
		for (int i = 0; i < size; ++i)
		{
			if (!Table[i].empty())
				std::cout << Table[i].type << " | " << Table[i].lexeme << " | " << Table[i].id << std::endl;
		}
	}

	void printFOut(std::string& filePath)
	{
		std::ofstream file(filePath);
		if (file.is_open())
		{
			for (int i = 0; i < size; ++i)
			{
				if (!Table[i].empty())
					file << Table[i].type << " | " << Table[i].lexeme << " | " << Table[i].id << std::endl;
			}
			file.close();
		}
		else
		{
			throw std::runtime_error("Could not open file for write");
		}
	}
};