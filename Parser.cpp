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

string_type hashtype(string const& inString){

	if (inString == "VARCHAR") return eChar;
	if (inString == "INTEGER") return eInt;
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
		delim = l.find(' ') + 1; //erase TABLE before calling parse_create
		l.erase(0, delim);
		parse_create(l);
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


//-------------------------------------------------------------------------------------------------------
void parse_type(string l, vector<attribute> attrVector, vector<string> primaryKey){

	while (l.size() > 2){
		string temp_type, temp_name;
		int temp_size;
		temp_name = l.substr(0, l.find(' ')); //get the name for attribute
		primaryKey.push_back(temp_name);
		l.erase(0, l.find(' ') + 1);
		temp_type = l.substr(0, l.find('R') + 1);//all the types end with R
		string_type type = hashtype(temp_type);

		switch (type){ //switch the type for attribute
		case eChar:
			temp_type = "Char";
			l.erase(0, l.find('(') + 1);//erase up to '(' before size
			temp_size = stoi(l.substr(0, l.find(')')));
			//attribute temp_attr(temp_name, temp_type, 0, temp_size);
			cout << "Parsed attribute name:" << temp_name << " type: " << temp_type << " size:" << temp_size << endl;
			l.erase(0, l.find(')') + 3);
			break;
		case eInt:
			temp_type = "Int";
			cout << "Parsed attribute name:" << temp_name << " type: " << temp_type << endl;
			l.erase(0, l.find(')') + 1);
			break;
		default:
			cout << "Something went wrong in parse_type switch(type)\n";
		}
		//l.erase(0, (l.find(" ") + 3));
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

void Parser::parse_create(string l){

	vector<attribute> attrVect;
	vector<string> primaryKey;

	size_t pos = l.find('(') + 1;
	string relation_name, typed_attribute_list, attribute_list;
	relation_name = l.substr(0, l.find(' '));
	l.erase(0, pos); //erase everything up to and including '("'
	string delimeter = ", ";
	pos = 0;
	typed_attribute_list = l.substr(0, l.find(" PRIMARY")) + "  ";
	cout << "typed attribute list: " << typed_attribute_list << endl;
	parse_type(typed_attribute_list, attrVect, primaryKey);
}


//---------------------------------------------------------------------------------------------------------------------
int main(){
	string test_command = "CREATE TABLE animals (name VARCHAR(20), kind VARCHAR(8), years INTEGER) PRIMARY KEY (name, kind);";
	Parser test(test_command);
	test_command = "CREATE TABLE species (kind VARCHAR(10)) PRIMARY KEY (kind);"; //removed the "" for testing
	Parser test2(test_command);
	/*test_command = "SHOW animals";
	Parser test3(test_command);
	test_command = "WRITE animals";
	Parser test4(test_command);
	test_command = "CLOSE animals";
	Parser test5(test_command);*/
}