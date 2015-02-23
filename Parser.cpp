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

void Parser::parse(){
	string temp = line.substr(0, line.find(' '));
	if (temp == "CREATE" || temp == "OPEN" || temp == "INSERT" || temp == "SHOW" || temp == "WRITE" || temp == "CLOSE" || temp == "EXIT")
		parse_command(line);
	else parse_query();
}

//--------------------------------------------------------------------------------------------------------
//Parser:: parse_command(string l)

void Parser::parse_command(string l){
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
		l.erase(0, l.find("INTO ") + 5);
		parse_insert(l);
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
//void parse_type(string l, vector<attribute> attrVector, vector<string> primaryKey)
void parse_type(string l, vector<string> primaryKey){

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
<<<<<<< HEAD
			temp_size = stoi(l.substr(0, l.find(')')));
=======
			temp_size = atoi(l.substr(0, l.find(')')).c_str());
			//attribute temp_attr(temp_name, temp_type, 0, temp_size);
>>>>>>> 08193623081b542dbf8c81a58213018356f84542
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
void trimQuote(string& s) {
	string temp = "";
	if (s[0] == '"' && s[s.length() - 1] == '"')
		temp = s.substr(1, s.length() - 2);
	s = temp;
}

//Parser::Validate Query objects
bool Parser::ValidateSelect() {
	for (int i = 0; i < query.size(); ++i) {
		for (int j = 0; j < query[i].attributes.size(); ++j) {
			string ops = query[i].attributes[j];
			// Primative check to ensure expected values obtained
			if (ops != "&&" && ops != "==" && ops != "!=" && ops != "||"
				&& ops != "<" && ops != ">" && ops != "<=" && ops != ">=")
				return false;
		}
	}
	return true;
}

//Parses select
bool Parser::parse_select(string select_string){
	string type = "";
	size_t pos = 0;
	pos = select_string.find(" ");
	type = select_string.substr(0, pos);
	select_string.erase(0, pos + 2);
	// While loop to parse all conditions of select
	while (select_string.length() != 0) {
		element select;
		// Get column name and erase from command
		select.query_type = type;
		select.column = select_string.substr(0, pos = select_string.find(" "));
		select_string.erase(0, select.column.length() + 1);
		// Get attribute value and erase from command
		select.attributes.push_back(select_string.substr(0, 2));
		select_string.erase(0, 3);
		// See if there is additional conditions
		pos = select_string.find(" ");
		if (pos == string::npos) {
			// No more conditions
			select.value = select_string.substr(0, select_string.find(")"));
			select_string.erase(0, select.value.length()+1);
		}
		else {
			// Additional conitions expected
			select.value = select_string.substr(0, pos);
			select_string.erase(0, pos+1);
			pos = select_string.find(" ");
			select.attributes.push_back(select_string.substr(0, pos));
			select_string.erase(0, pos+1);
		}
		trimQuote(select.value);
		query.push_back(select);
	}
	return ValidateSelect();
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
			bool test = parse_select(select_string);
			line.erase(0, pos);
		}
		else if (temp == "rename" || temp == "project"){
			pos = line.find(')') + 1;
			string list_string = line.substr(0, pos);
			element list = parse_list(list_string);
			query.push_back(list);
			line.erase(0, pos);
		}
		else { // Relational algebra
			// Stores relation on left hand side
			fromName = line.substr(0, pos);
			line.erase(0,pos + 1);
			// Stores which operation to perform
			op = line[0];
			line.erase(0, line.find(' ') + 1);
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------
//Parser:: parse_create(string l)

void Parser::parse_create(string l){

	//vector<attribute> attrVect;
	vector<string> primaryKey;

	size_t pos = l.find('(') + 1;
	string relation_name, typed_attribute_list, attribute_list;
	relation_name = l.substr(0, l.find(' '));
	l.erase(0, pos); //erase everything up to and including '("'
	string delimeter = ", ";
	pos = 0;
	typed_attribute_list = l.substr(0, l.find(" PRIMARY")) + "  ";
	cout << "typed attribute list: " << typed_attribute_list << endl;
	//parse_type(typed_attribute_list, attrVect, primaryKey);
	parse_type(typed_attribute_list, primaryKey);
}

//--------------------------------------------------------------------------------------------------------------------
//Parser:: parse_insert(string l)

void Parser::parse_insert(string l){

	string relation_name, expr, temp_literal;
	vector<string> literals;
	relation_name = l.substr(0, l.find(' '));
	//cout << "INSERT INTO (Relation: " << relation_name << ") VALUES FROM (";
	l.erase(0, l.find("FROM") + 5);
	//cout << l << ")\n";
	expr = l.substr(0, l.find(" "));
	//cout << "EXPR: " << expr << endl;
	if (expr != "RELATION"){
		//need to get attributes of relation_name
		l.erase(0, l.find('(') + 1); // Joe, cat, 4);
		l.erase(l.find(')'), l.find(';') + 1);
		l += " ";
		//cout << "Erased ); at the end of: " << l << endl;
		while (l.size() > 0){
			temp_literal = l.substr(0, l.find(','));
			literals.push_back(temp_literal);
			l.erase(0, l.find(' ') + 1);
		}
		//for (int i = 0; i < literals.size(); i++){ cout << literals[i] << " "; }
	}
	else{ //needs to parse expr
		l.erase(0, l.find("RELATION ") + 9);
		expr = l.substr(0, l.find(';'));
		//cout << expr << endl;
	}
}


//---------------------------------------------------------------------------------------------------------------------
/*int main(){
	string test_command = "CREATE TABLE animals (name VARCHAR(20), kind VARCHAR(8), years INTEGER) PRIMARY KEY (name, kind);";
	Parser test(test_command);
	test_command = "CREATE TABLE species (kind VARCHAR(10)) PRIMARY KEY (kind);"; //removed the "" for testing
	Parser test2(test_command);
	test_command = "INSERT INTO animals VALUES FROM (Joe, cat, 4);";
	Parser test3(test_command);
	test_command = "INSERT INTO species VALUES FROM RELATION project (kind) animals;";
	Parser test4(test_command);
	test_command = "CLOSE animals";
	Parser test5(test_command);
}*/
