#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <cctype>

#include "Log.hpp"

enum ARG_TYPE { ARG_STRING , ARG_NUMBER , ARG_CVAR , ARG_UNDEF };
enum NUM_TYPE { NUM_INT , NUM_FLOAT };
// intermediate number
struct INumber {
	INumber() { }
	INumber(long long i): type(NUM_INT), __int__(i) { }
	INumber(double d): type(NUM_FLOAT), __double__(d) { }

	NUM_TYPE type;
	union {
		long long __int__;
		double    __double__;
	};

	long long ToInt() const;
	double    ToFloat() const;
};

struct Argument {
	Argument() { }
	Argument(long long i, std::string _label=""): type(ARG_NUMBER), num(i), label(_label) { }
	Argument(double d, std::string _label=""): type(ARG_NUMBER), num(d), label(_label) { }
	Argument(INumber n, std::string _label=""): type(ARG_NUMBER), num(n), label(_label) { }
	Argument(ARG_TYPE _type, const std::string& _str, std::string _label=""):
	  type(_type), str(_str), label(_label) { }

	ARG_TYPE type = ARG_UNDEF;

	INumber num;
	std::string str = "";
	std::string label;

	long long   ToInt() const;
	double      ToFloat() const;
	std::string ToString() const;
};

bool ArgIsNum(const Argument& arg);
bool ArgIsNumRange(const Argument& arg, double low, double high);
bool ArgIsString(const Argument& arg);
template <typename T>
bool ArgIsStringElement(const Argument& arg, const T& strs) { 
	if (!ArgIsString(arg)) return false;
	std::string str = arg.ToString();
	for (auto i = strs.begin(); i != strs.end(); ++i)
		if (*i == str) return true;
	return false;
}

struct Command {
	Command() { }
	Command(const std::string& _command, const std::vector<Argument>& _args):
	  command(_command), args(_args) { }
	std::string command;
	std::vector<Argument> args;
};

// CONSOLE VARIABLES
extern std::map<std::string, Argument> CVARS;
// GET CONSOLVE VARIABLES
Argument * GetCVar(const std::string& id);
// GET CONSOLVE VARIABLE AND RETURN DEFAULT VALUE IF NON-EXISTANT
long long GetCVarInt(const std::string& id, long long def = 0);
double    GetCVarFloat(const std::string& id, double def = 0.0);
std::string GetCVarString(const std::string& id, std::string def = "");
#define GetCVarDouble GetCVarFloat

namespace Parser {

	// returns 1 on success, 0 on error
	// - on success Argument& arg is set as the constructed command
	int ParseCommand(const std::string& string, Command& com);

	extern std::string ErrorMsg;

	extern INumber l_number;
	extern std::string l_string; // string for identifier and strings

	enum TOKEN {
		TOK_EOF = -1,
		TOK_IDENTIFIER = -2,
		TOK_NUMBER = -3,
		TOK_STRING = -4,

		ERROR = -127
	};

	void SetupLexer(std::string str);
	int GetNextToken();
	int __GetNextToken();
	int GetCurrentToken();
	extern int last_tok;

	int LexIdentifier();
	int LexNumber();
	int LexString();

	extern bool error_flag;
	Argument ParseArgument();
	Argument ParseNumber();
	Argument ParseString();
	Argument ParseIdentifier();
	Argument ParseCVar();
};
