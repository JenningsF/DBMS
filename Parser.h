#include <string>
#include <iostream>
#include <string>
//Replace with new header file
#include "DBengine.h"

using namespace std;

//move to new header file
enum string_command{eOpen, eClose, eInsert, eCreate, eWrite, eExit, eUpdate, eDelete, eShow};
string_command hashit(string const& inString);
enum string_type{eChar, eInt};
string_type hashtype(string const& inString);

//move to new header file
struct element {
	string query_type;
	string column;
	string value;
	vector<string> attributes;
};

class Parser {
private:
	/*Holds input when constructor is called*/
	string line;
	/*Parameters used for queries*/
	string viewName;
	string fromName;
	char op;
	vector<element> query;
	/*Parameters used for queries and commands*/
	
	/*Parameters used for commands*/
	
public:
	Parser();
	~Parser();
	//Add bool return
	void parse(string line);
	//Rewrite this function to utilize the line variable
	// getting rid of string l
	void parse_command(string l);
	void parse_create(string l);
	void parse_insert(string l);
	//following functions used to parse query
	bool ValidateSelect();
	bool parse_select(string select_string);
	element parse_list(string list_string);
	void parse_query();
	
	//Create get functions for private members
}; 




