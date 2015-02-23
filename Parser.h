#include <string>
#include <iostream>
#include <string>
#include "DBengine.h"

using namespace std;

enum string_command{eOpen, eClose, eInsert, eCreate, eWrite, eExit, eUpdate, eDelete, eShow};
string_command hashit(string const& inString);
enum string_type{eChar, eInt};
string_type hashtype(string const& inString);

struct element {
	string query_type;
	string column;
	string value;
	vector<string> attributes;
};

class Parser {
private:
	string line;
	string viewName;
	string fromName;
	char op;
	vector<element> query;
public:
	Parser();
	Parser(string l) : line(l) {
		parse();
	}
	void parse();
	void parse_command(string l);
	void parse_create(string l);
	void parse_insert(string l);
	//following functions used to parse query
	bool ValidateSelect();
	bool parse_select(string select_string);
	element parse_list(string list_string);
	void parse_query();
}; 




