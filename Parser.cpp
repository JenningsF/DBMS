#include "Parser.h"

using namespace std;


string_command hashit(string const& inString){ //string type can's be used in switch statement 
	if (inString == "OPEN") return eOpen;
	if (inString == "CLOSE") return eClose;
	if (inString == "INSERT") return eInsert;
	if (inString == "CREATE") return eCreate;
	if (inString == "WRITE") return eWrite;
	if (inString == "EXIT") return eExit;
	if (inString == "UPDATE") return eUpdate;
	if (inString == "DELETE") return eDelete;
	if (inString == "SHOW") return eShow;
}

//------------------------------------------------------------------------------------------------------
//Parser:: parse(string l)

void Parser:: parse(string l){
	string temp = l.substr(0, l.find(' '));
	if (temp == "CREATE" || temp == "OPEN" || temp == "INSERT" || temp == "SHOW" || temp == "WRITE" || temp == "CLOSE" || temp == "EXIT")
		parse_command(l);
	else parse_query(l);
}

//--------------------------------------------------------------------------------------------------------
//Parser:: parse_command(string l)

void Parser:: parse_command(string l){
	string temp = l.substr(0, l.find(' '));
	size_t delim = l.find(' ') + 1;
	l.erase(0, delim);
	string relation_name, pause;
	string_command command = hashit(temp);
	switch (command){
	case eCreate:
		cout << "Call create for: " << l << endl;
		cin >> pause;
		//delim = l.find(' ') + 1; //erase TABLE before calling parse_create
		//l.erase(0, delim);
		//parse_create(l);
		break;
	case eInsert:
		cout << "Call insert for: " << l << endl;
		cin >> pause;
		break;
	case eShow:
		relation_name = l.substr(0, l.find(' '));
		cout << "Call show for relation: " << relation_name << endl;
		cin >> pause;
		break;
	case eWrite:
		relation_name = l.substr(0, l.find(' '));
		cout << "Call write to table name: " << relation_name << endl;
		cin >> pause;
		break;
	case eClose:
		relation_name = l.substr(0, l.find(' '));
		cout << "Call close for file: " << relation_name << endl;
		cin >> pause;
		break;
	case eExit:
		cout << "Call exit" << endl;
		cin >> pause;
		//exit();
		break;
	default:
		cout << "Entered switch(command), something went wrong\n";
		cin >> pause;
		break;
	}
}

//--------------------------------------------------------------------------------------------------------
//Parser::parse_query(string l)

vector<element> Parser::parse_select(string select)
{
	string temp = select.find('(') - select;
	cout << temp;
}

vector<element> Parser::parse_query(string l)
{
	//Parse input
	//calls function listed above to parse smaller objects
	//Use while loop and check for new atomic expression until end of line
	//Append the parsed command to the vector initialized within parse_query 
	//vector<element> parsed_command;
}

//--------------------------------------------------------------------------------------------------------------------
//Parser:: parse_create(string l)
/*
void Parser::parse_create(string l){

	size_t pos = l.find('(') + 1;
	string relation_name, typed_attribute_list, attribute_list;
	relation_name = l.substr(0, l.find(' '));
	l.erase(0, pos); //erase everything up to and including '("'
	cout << "\n" << l << endl;
	size_t close_paren = l.find(")");
	string delimeter = ", ";
	pos = 0;
	cout << typed_attribute_list << endl;
}
*/
//---------------------------------------------------------------------------------------------------------------------
int main(){
	Parser test;
	test.parse_select("select(kind = dog)");
	// string test_command = "CREATE TABLE animals (name VARCHAR(20), kind VARCHAR(8), years INTEGER) PRIMARY KEY (name, kind);";
	// Parser test(test_command);
	// test_command = "INSERT INTO animals VALUES FROM (Joe, cat, 4);"; //removed the "" for testing
	// Parser test2(test_command);
	// test_command = "SHOW animals";
	// Parser test3(test_command);
	// test_command = "WRITE animals";
	// Parser test4(test_command);
	// test_command = "CLOSE animals";
	// Parser test5(test_command);
	return 0;
}