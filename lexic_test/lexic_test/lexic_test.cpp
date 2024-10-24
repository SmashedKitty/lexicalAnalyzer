#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <set>
#include <string>
#include <iostream>
#include <fstream>

class dfa
{
private:
	int countStates; // количество состояний автомата
	std::vector<char> alphabet; // входной алфавит
	std::vector<int> finalStates; // множество допускающих состояний
	std::vector<std::vector<int>> transitFunction; // функция переходов 
	// transitFunction[i][j] = k означает, что из i-го состояния по j-му символу входного алфавита автомат переходит в состояние k
	int getState(int state, char c);
	std::set<int> inverse(int state, int k);
public:
	dfa(int countStates, std::vector<char> alphabet, std::vector<int> finalStates, std::vector<std::vector<int>> transitFunction);
	bool isAccept(std::string s);
	dfa minimize();
	std::vector<std::vector<int>> quotientSet();
};
dfa::dfa(int countStates, std::vector<char> alphabet, std::vector<int> finalStates, std::vector<std::vector<int>> transitFunction)
{
	this->countStates = countStates;
	this->alphabet = alphabet;
	this->finalStates = finalStates;
	this->transitFunction.resize(countStates);
	for (int i = 0; i < countStates; i++)
		this->transitFunction[i] = transitFunction[i];
}


// Находит состояние автомата по текущему состоянию state и входному символу c
int dfa::getState(int state, char c)
{
	int i = 0;
	while (alphabet[i] != c)
	{
		i++;
		if (i == alphabet.size())
		{
			throw std::exception("Wrong symbol");
		}
	}
	return transitFunction[state][i];
}


// Проверяет, допускается ли строка s автоматом
bool dfa::isAccept(std::string s)
{
	int state = 0;
	for (char c : s)
	{
		state = getState(state, c);
		if (state == -1)
			return false;
	}
	if (std::find(finalStates.begin(), finalStates.end(), state) != finalStates.end())
		return true;
	return false;
}


// Находит все состояния, из которых автомат переходит по k-му алфавитному символу в состояние state
std::set<int> dfa::inverse(int state, int k)
{
	std::set<int> result;
	for (int i = 0; i < countStates; i++)
		if (transitFunction[i][k] == state)
			result.insert(i);
	return result;
}


// Строит множество классов эквивалентности на множестве состояний (нужно для построения минимального автомата)
std::vector<std::vector<int>> dfa::quotientSet()
{
	std::vector<std::vector<int>> table(countStates, std::vector<int>(countStates, 0));
	std::queue<std::pair<int, int>> q;
	for (int i = 0; i < countStates; i++)
	{
		if (std::find(finalStates.begin(), finalStates.end(), i) == finalStates.end())
			for (int x : finalStates)
			{
				table[i][x] = 1;
				table[x][i] = 1;
				q.push(std::pair<int, int>(i, x));
			}
	}
	while (!q.empty())
	{
		std::pair<int, int> p = q.front();
		q.pop();
		int x = p.first;
		int y = p.second;
		for (int i = 0; i < alphabet.size(); i++)
		{
			auto s1 = inverse(x, i);
			auto s2 = inverse(y, i);
			if (s1.size() != 0 && s2.size() != 0)
			{
				for (int j : s1)
					for (int k : s2)
						if (table[j][k] != 1)
						{
							table[j][k] = 1;
							table[k][j] = 1;
							q.push(std::pair<int, int>(j, k));
						}
			}
		}
	}
	std::vector<int> mark(countStates, 0);
	std::vector<std::vector<int>> result;
	std::vector<int> t;
	for (int i = 0; i < countStates; i++)
	{
		if (mark[i] == 0)
		{
			mark[i] = 1;
			t.push_back(i);
			for (int j = i + 1; j < countStates; j++)
				if (table[i][j] == 0)
				{
					mark[j] = 1;
					t.push_back(j);
				}
			result.push_back(t);
			t.clear();
		}

	}
	return result;
}


// Строит эквивалентный ДКА с минимальным числом состояний
dfa dfa::minimize()
{
	auto q = quotientSet();
	std::vector<int> minFinalStates;
	int minCountStates = q.size();
	std::vector<std::vector<int>> minTransitFunction(minCountStates, std::vector<int>(alphabet.size(), -1));
	for (int i = 0; i < q.size(); i++)
		for (int j : q[i])
		{
			if (std::find(finalStates.begin(), finalStates.end(), j) != finalStates.end())
			{
				minFinalStates.push_back(i);
				break;
			}
		}
	for (int i = 0; i < minCountStates; i++)
	{
		for (int j = 0; j < alphabet.size(); j++)
		{
			int k = transitFunction[q[i][0]][j];
			for (int l = 0; l < minCountStates; l++)
				if (std::find(q[l].begin(), q[l].end(), k) != q[l].end())
				{
					minTransitFunction[i][j] = l;
					break;
				}
		}
	}
	auto minDfa = dfa(minCountStates, alphabet, minFinalStates, minTransitFunction);
	return minDfa;
}

class FiniteAutomats
{
public:
	bool isKeyword(const std::string& lexeme);
	bool isIdentifier(const std::string& lexeme);
	bool isInteger(const std::string& lexeme);
	bool isOperator(const std::string& lexeme);
	bool isDelimiter(const std::string& lexeme);
};

bool FiniteAutomats::isKeyword(const std::string& lexeme)//должен быть наивыший приоритет определниея чтобы не путатать с ID 
{
	int countStates = 34; // количество состояний автомата /**/
	std::vector<char> alphabet = {
		'p'/*0*/, 'r'/*1*/,'o'/*2*/,'c'/*3*/,'e'/*4*/,'d'/*5*/,'u'/*6*/,
		'b'/*7*/,'g'/*8*/,'i'/*9*/,'n'/*10*/,
		'v'/*11*/, 'a'/*12*/,
		's'/*13*/,
		'f'/*14*/,
		't'/*15*/

	}; // входной алфавит

	std::vector<int> finalStates = { 9, 14, 17, 20, 24, 26, 33 }; // множество допускающих состояний
	std::vector<std::vector<int>> transitFunction =
	{
		//  {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		//   p   r   o   c   e   d   u   b   g   i   n   v   a   s   f   t    	
			{ 1, -1, 25, 21, 15, -1, -1, 10, -1, 27, -1, 18, -1, -1, -1, -1},//0
			{-1,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//1
			{-1, -1,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//2
			{-1, -1, -1,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//3
			{-1, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//4
			{-1, -1, -1, -1, -1,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//5
			{-1, -1, -1, -1, -1, -1,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1},//6
			{-1,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//7
			{-1, -1, -1, -1,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//8
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//9  final
			{-1, -1, -1, -1, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//10
			{-1, -1, -1, -1, -1, -1, -1, -1, 12, -1, -1, -1, -1, -1, -1, -1},//11
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, 13, -1, -1, -1, -1, -1, -1},//12
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 14, -1, -1, -1, -1, -1},//13
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//14 final
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 16, -1, -1, -1, -1, -1},//15
			{-1, -1, -1, -1, -1, 17, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//16
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//17 final
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 19, -1, -1, -1},//18
			{-1, 20, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//19
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//20 final
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 22, -1, -1, -1},//21
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 23, -1, -1},//22
			{-1, -1, -1, -1, 24, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//23
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//24 final
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 26, -1},//25
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//26 final
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 28, -1, -1, -1, -1, -1},//27
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 29},//28
			{-1, -1, -1, -1, 30, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//29
			{-1, -1, -1, -1, -1, -1, -1, -1, 31, -1, -1, -1, -1, -1, -1, -1},//30
			{-1, -1, -1, -1, 32, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},//31
			{-1, 33, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, //32
			{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1} //33 final

	};
	dfa KeyWord(countStates, alphabet, finalStates, transitFunction);
	return KeyWord.isAccept(lexeme);
}

bool FiniteAutomats::isIdentifier(const std::string& lexeme)
{
	int countStates = 2;
	std::vector<char> alphabet = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
	};
	std::vector<int> finalStates = { 0, 1 };
	std::vector<std::vector<int>> transitFunction =
	{
		//A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};
	dfa Identifier(countStates, alphabet, finalStates, transitFunction);
	return Identifier.isAccept(lexeme);
}
bool FiniteAutomats::isInteger(const std::string& lexeme)
{
	int countStates = 1;
	std::vector<char> alphabet = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	std::vector<int> finalStates = { 0 };
	std::vector<std::vector<int>> transitFunction =
	{
		//0, 1, 2, 3, 4, 5, 6, 7, 8, 9
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	};
	dfa Integer(countStates, alphabet, finalStates, transitFunction);
	return Integer.isAccept(lexeme);
}

bool FiniteAutomats::isOperator(const std::string& lexeme)
{
	int countStates = 5;
	std::vector<char> alphabet = { ':', '=', '+', '-' };
	std::vector<int> finalStates = {2, 3, 4, 5 };
	std::vector<std::vector<int>> transitFunction =
	{
		// :,  =,  +,  - 
		{  1, -1,  3,  4},//0
		{ -1,  2, -1, -1},//1 
		{ -1, -1, -1, -1},//2 final
		{ -1, -1, -1, -1},//3 final
		{ -1, -1, -1, -1} //4 final 
	};
	dfa Operator(countStates, alphabet, finalStates, transitFunction);
	return Operator.isAccept(lexeme);
}

bool FiniteAutomats::isDelimiter(const std::string& lexeme)
{
	int countStates = 5;
	std::vector<char> alphabet = { ';', ':', ',', '(', ')' };
	std::vector<int> finalStates = {1, 2, 3, 4, 5 };
	std::vector<std::vector<int>> transitFunction =
	{
		// ';' ':'  ',' '(', ')' 
		{   1,  2,   3,  4,  5},//0
		{  -1, -1,  -1, -1, -1},//1 final
		{  -1, -1,  -1, -1, -1},//2 final
		{  -1, -1,  -1, -1, -1},//3 final
		{  -1, -1,  -1, -1, -1},//4 final
		{  -1, -1,  -1, -1, -1},//5 final
	};
	dfa Delimiter(countStates, alphabet, finalStates, transitFunction);
	return Delimiter.isAccept(lexeme);
}

//bool isInteger(const std::string& lexeme);
//bool isOperator(const std::string& lexeme);
//bool isDelimiter(char ch);
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



int main() {
	std::ifstream file;
	std::string filePath = "test.txt";
	std::string s = "out.txt";
	lexer A(filePath);
	A.printH();
	A.printHfile(s);
	

	

	return 0;
}