#include <string>
#include <cstdlib>
#include <iostream>
#include <string>
//Replace with new header file
#include "Attributes.h"

using namespace std;


class Parser {
private:
	/*Holds input when constructor is called*/
	string line;
	/*Parameters used for queries*/
	string viewName;
	string fromName;
	string_command command;
	vector<element> query;
	/*Parameters used for queries and commands*/
	
	/*Parameters used for commands*/
	
public:
	Parser();
	// Getters
	string getViewName();
	string getFromName();
	string_command getCommand();

	//Add bool return
	void parse(string line);
	//Rewrite this function to utilize the line variable
	// getting rid of string l
	void parse_command();
	void parse_create();
	void parse_insert();
	//following functions used to parse query
	bool ValidateSelect();
	bool parse_select(string select_string);
	element parse_list(string list_string);
	void parse_query();
	
	//Create get functions for private members
}; 
