#include <string>
#include <iostream>
#include <string>
#include "DBengine.h"

using namespace std;

enum string_command{eOpen, eClose, eInsert, eCreate, eWrite, eExit, eUpdate, eDelete, eShow};
string_command hashit(string const& inString);
enum string_type{eChar, eInt};
string_type hashtype(string const& inString);

class Parser {
private:
	string line;
	string viewName;
public:
	Parser();
	Parser(string l) : line(l) {
		parse(l);
	}
	string get_atomic_exp() {
		viewName = line.substr(0,line.find(' '));
		line.erase(0,line.find(' ') + 1);

	}
	void parse(string l);
	void parse_command(string l);
	void parse_create(string l);
	//void parse_query() {}

}; 




