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

void Parser:: parse(){
	string temp = line.substr(0, line.find(' '));
	if (temp == "CREATE" || temp == "OPEN" || temp == "INSERT" || temp == "SHOW" || temp == "WRITE" || temp == "CLOSE" || temp == "EXIT")
		parse_command(line);
	else parse_query();
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
		l.erase(0, l.find(' ') + 1);
		temp_type = l.substr(0, l.find('R') + 1);//all the types end with R
		string_type type = hashtype(temp_type);

		switch (type){ //switch the type for attribute
		case eChar:
			temp_type = "Char";
			l.erase(0, l.find('(') + 1);//erase up to '(' before size
			temp_size = stoi(l.substr(0, l.find(')')));
			//attribute temp_attr(temp_name, temp_type, 0, temp_size);
			primaryKey.push_back(temp_name);
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
	cout << "Primary key holds: ";
	for (int i = 0; i < primaryKey.size(); i++)
	{
		cout << primaryKey[i] << " ";
	}
	cout << endl;
}

//--------------------------------------------------------------------------------------------------------
//Parser::parse_query

//Parses select
element Parser::parse_select(string select_string){
	element select;
	size_t pos = 0;
	pos = select_string.find(" ");
	select.query_type = select_string.substr(0, pos);
	select_string.erase(0, pos + 2);
	select.column = select_string.substr(0, pos = select_string.find(" "));
	select_string.erase(0, pos + 5);
	select.value = select_string.substr(0, pos = select_string.find("\""));
	select_string.erase(0, pos + 3);
	return select; 
}

//Parses rename and project
element Parser::parse_list(string list_string) {
	element list;
	size_t pos = 0;
	pos = list_string.find(" ");
	list.query_type = list_string.substr(0, pos);
	list_string.erase(0, pos + 2);
	while (true){
		pos = list_string.find(',');
		if (pos == string::npos){
			pos = list_string.find(')');
			string temp = list_string.substr(0, pos);
			list.attributes.push_back(temp);
			break;
		}
		else {
			string temp = list_string.substr(0, pos);
			list.attributes.push_back(temp);
			list_string.erase(0, pos + 2);
		}
	}
	return list;
}

void Parser::parse_query() {
	//Parse input
	//calls function listed above to parse smaller objects
	//Use while loop and check for new atomic expression until end of line
	//Append the parsed command to the vector initialized within parse_query 
	//vector<element> parsed_command;
	
	//Strip off viewName
	size_t pos = 0;
	pos = line.find(" ");
	viewName = line.substr(0,pos);
	line.erase(0,pos + 4);
	//Process Query
	while(pos != string::npos){
		pos = line.find(" ");
		string temp = line.substr(0,pos);
		if (temp == "select"){
			pos = line.find(')') + 1;
			string select_string = line.substr(0, pos);
			element sel = parse_select(select_string);
			query.push_back(sel);
			line.erase(0, pos);
		}
		else if (temp == "rename" || temp == "project"){
			pos = line.find(')') + 1;
			string list_string = line.substr(0, pos);
			element list = parse_list(list_string);
			query.push_back(list);
			line.erase(0, pos);
		}
		else {
			line.erase(0,pos + 1);
		}
	}
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