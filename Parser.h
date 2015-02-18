#include <string>
#include "DBengine.h"

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
	bool parse(string l) {		
		string temp = line.substr(0,line.find(' '));
		if (temp == "CREATE" || temp == "OPEN" ....)
			parse_command();
		else parse_query();
	}
	void parse_command();
	void parse_query() {

	}

};