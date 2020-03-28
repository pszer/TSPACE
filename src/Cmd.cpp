#include "Cmd.hpp"

std::map<std::string, Argument> CVARS;

Argument * GetCVar(const std::string& id) {
	auto f = CVARS.find(id);
	if (f == CVARS.end()) return nullptr;
	else return &(f->second);
}

long long GetCVarInt(const std::string& id, long long def) {
	auto a = GetCVar(id);
	if (a == nullptr) return def;
	else return a->ToInt();
}

double GetCVarFloat(const std::string& id, double def) {
	auto a = GetCVar(id);
	if (a == nullptr) return def;
	else return a->ToFloat();
}

std::string GetCVarString(const std::string& id, std::string def) {
	auto a = GetCVar(id);
	if (a == nullptr) return def;
	else return a->ToString();
}

long long INumber::ToInt() const {
	if (type == NUM_INT)
		return __int__;
	return static_cast<long long>(__double__);
}

double INumber::ToFloat() const {
	if (type == NUM_INT)
		return static_cast<double>(__int__);
	return __double__;
}

long long Argument::ToInt() const {
	if (type == ARG_NUMBER) return num.ToInt();
	else if (type == ARG_CVAR) return GetCVarInt(str);
	else return std::strtoll(str.c_str(), NULL, 10);
}

double Argument::ToFloat() const {
	if (type == ARG_NUMBER) return num.ToFloat();
	else if (type == ARG_CVAR) return GetCVarFloat(str);
	else return std::strtod(str.c_str(), NULL);
}

std::string Argument::ToString() const {
	if (type == ARG_STRING) return str;
	else if (type == ARG_CVAR) return GetCVarString(str); 
	else if (num.type == NUM_INT)
		return std::to_string(num.__int__);
	else
		return std::to_string(num.__double__);
}

bool ArgIsNum(const Argument& arg) {
	switch (arg.type) {
	case ARG_NUMBER:
		return true;
	case ARG_STRING:
		{
		const char * c = arg.ToString().c_str();
		char * endptr;
		strtod(c, &endptr);
		if (*endptr) return false;
		return true;
		}
	case ARG_CVAR:
		{
		Argument * cvar_arg = GetCVar(arg.str);
		if (!cvar_arg) return false;
		return ArgIsNum(*cvar_arg);
		}
	default:
		return false;
	}
}

bool ArgIsNumRange(const Argument& arg, double low, double high) {
	if (!ArgIsNum(arg)) return false;
	double val = arg.ToFloat();
	return val >= low && val <= high;
}

bool ArgIsString(const Argument& arg) {
	switch (arg.type) {
	case ARG_CVAR:
		{
		Argument * cvar_arg = GetCVar(arg.str);
		if (!cvar_arg) return false;
		return ArgIsString(*cvar_arg);
		}
	default:
		return true;
	}
}

/*bool ArgIsStringElement(const Argument& arg, const std::vector<std::string>& strs) { 
	if (!ArgIsString(arg)) return false;
	std::string str = arg.ToString();
	for (auto i = strs.begin(); i != strs.end(); ++i)
		if (*i == str) return true;
	return false;
}*/

using namespace Parser;

INumber Parser::l_number;
std::string Parser::l_string;
std::string Parser::ErrorMsg = "";
bool Parser::error_flag = false;

std::stringstream l_sstr;
int l_char, last_token;

void Parser::SetupLexer(std::string str) {
	l_sstr = std::stringstream(str);
	l_char = l_sstr.get();
}

int Parser::GetCurrentToken() {
	return last_token;
}

int Parser::GetNextToken() {
	last_token = __GetNextToken();
	if (last_token == ERROR) error_flag = true;
	return last_token;
}

int Parser::__GetNextToken() {
	error_flag = false;

	// ignore whitespaces
	while (std::isspace(l_char)) {
		l_char = l_sstr.get();
	}

	// eof
	if (l_sstr.eof()) return TOK_EOF;
	
	// if identifier
	if (std::isalpha(l_char)) {
		return LexIdentifier();
	}

	// if number
	if (std::isdigit(l_char) || l_char == '-') {
		return LexNumber();
	}

	// if string
	if (l_char == '"') {
		return LexString();
	}

	// return any characters not recognised as tokens
	int old_char = l_char;
	l_char = l_sstr.get();
	return old_char;
}

int Parser::LexIdentifier() {
	Parser::l_string = "";
	while (!std::isspace(l_char) && l_char != ':' && !l_sstr.eof())
	{
		Parser::l_string += l_char;
		l_char = l_sstr.get();
	}

	return TOK_IDENTIFIER;
}

int Parser::LexNumber() {
	std::string str;

	bool is_int = true;
	do {
		str += l_char;
		l_char = l_sstr.get();	
		if (l_char == '.') is_int = false;
	} while (std::isdigit(l_char) || l_char == '.');

	try {
		if (is_int)
			l_number = INumber(std::stoll(str));
		else
			l_number = INumber(std::stod(str));
	} catch (...) {
		return ERROR;
	}

	return TOK_NUMBER;
}

int Parser::LexString() {
	Parser::l_string = "";

	while (true) {
		l_char = l_sstr.get();
		if (l_sstr.eof()) return ERROR; // no matching "
		if (l_char == '"') break;       // matching " found
			
		l_string += l_char;
	}

	l_char = l_sstr.get();
	return TOK_STRING;
}

int Parser::ParseCommand(const std::string& string, Command& _com) {
	SetupLexer(string);
	Command command; // do not overwrite _com in case of error

	ErrorMsg = "";
	error_flag = false;

	int tok = GetNextToken();
	// command must begin with command name 
	if (tok != TOK_IDENTIFIER) {
		ErrorMsg = "Command must begin with command identifier";
		return 0;
	}

	command.command = Parser::l_string;

	GetNextToken();
	while ((tok = GetCurrentToken()) != TOK_EOF) {
		Argument arg = ParseArgument();
		if (error_flag) return 0;
		command.args.push_back(std::move(arg));
	}

	_com = command;
	return 1;
}

bool error_flag = false;;
Argument Parser::ParseArgument() {
	switch (GetCurrentToken()) {
	case ERROR:
		return Argument();

	case TOK_NUMBER:
		return ParseNumber();
	case TOK_STRING:
		return ParseString();
	case TOK_IDENTIFIER:
		return ParseIdentifier();
	case '$':
		return ParseCVar();
	default:
		ErrorMsg = "Unexpected token in command";
		error_flag = true;
		return Argument();
	}
}

Argument Parser::ParseNumber() {
	Argument arg(Parser::l_number);
	GetNextToken();
	return arg;
}

Argument Parser::ParseString() {
	Argument arg(ARG_STRING, Parser::l_string);
	GetNextToken();
	return arg;
}

Argument Parser::ParseIdentifier() {
	std::string id = Parser::l_string;
	if (GetNextToken() != ':') {
		Argument arg(ARG_STRING, id);
		return arg;
	}

	if (GetNextToken() == TOK_IDENTIFIER) {
		ErrorMsg = "An argument cannot have two labels";
		error_flag = true;
		return Argument();
	} else {
		Argument arg = ParseArgument();
		arg.label = id;
		return arg;
	}
}

Argument Parser::ParseCVar() {
	GetNextToken(); // eat '$'
	if (GetCurrentToken() != TOK_IDENTIFIER) {	
		ErrorMsg = "A CVar name can only be a string";
		error_flag = true;
		return Argument();
	}

	Argument arg(ARG_CVAR, Parser::l_string);
	GetNextToken();
	return arg;
}
