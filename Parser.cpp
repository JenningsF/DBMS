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

Parser::Parser() {
	command = ERROR;
}

//------------------------------------------------------------------------------------------------------
//Parser:: getViewName() 
string Parser::getViewName() {
	return viewName;
}

string Parser::getFromName() {
	return fromName;
}

string_command Parser::getCommand() {
	return command;
}

string Parser::getTableName() {
	return tableName;
}

vector<attribute> Parser::getAttributes() {
	return attrVect;
}

vector<string> Parser::getPrimaryKeys() {
	return primaryKeys;
}

//------------------------------------------------------------------------------------------------------
//Parser:: parse(string l)

void Parser::parse(string l){
	line = l;
	string temp = line.substr(0, line.find(' '));
	if (temp == "CREATE" || temp == "OPEN" || temp == "INSERT" || temp == "SHOW" || temp == "WRITE" || temp == "CLOSE" || temp == "EXIT")
		parse_command();
	else parse_query();
}

//--------------------------------------------------------------------------------------------------------
//Parser:: parse_command(string l)
void Parser::parse_command(){
	string temp = line.substr(0, line.find(' '));
	size_t delim = line.find(' ') + 1;
	line.erase(0, delim);
	string relation_name, pause;
	string_command command = hashit(temp);
	switch (command){
	case eCreate: //good
		delim = line.find(' ') + 1; //erase TABLE before calling parse_create
		line.erase(0, delim);
		parse_create();
		break;
	case eInsert:
		cout << "Call insert for: " << line << endl;
		cin >> pause;
		line.erase(0, line.find("INTO ") + 5);
		parse_insert(line);
		break;
	case eShow:
		relation_name = line.substr(0, line.find(' '));
		cout << "Call show for relation: " << relation_name << endl;
		cin >> pause;
		break;
	case eWrite: //good
		relation_name = line.substr(0, line.find('\0'));
		break;
	case eClose:
		relation_name = line.substr(0, line.find(' '));
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

			line.erase(0, line.find(' ') + 1);
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------
//Parser:: parse_create(string l)

void convertType(string &type) {
	if(type == "VARCHAR" || type == "varchar") {
		type = "string";
	}
	else if(type == "INTEGER" || type == "integer") {
		type = "int";
	}
	else if(type == "DOUBLE" || type == "double") {
		type = "double";
	}
	return;
}

void Parser::parse_create(){
	//size_t pos = line.find('(') + 1;
	//string relation_name, typed_attribute_list, attribute_list;
	//Temp Variables
	string name, type;
	int size = 0;
	//Parser
	bool isDone = false;
	while(!isDone) {
		size_t pos = line.find(' ');
		tableName = line.substr(0, pos);
		line.erase(0, pos + 2); //erase tableName from line and first '('
		pos = line.find(' ');
		name = line.substr(0,pos); //attribute name
		line.erase(0, pos + 1);
		pos = line.find(' ');
		if(line.substr(pos-2, pos-1) == ")") {//case for when there is a specified sized
			pos = line.find('(');
			type = line.substr(0,pos);
			convertType(type);
			line.erase(0, pos + 1);
			pos = line.find(')');
			size = atoi(line.substr(0,pos).c_str());
			pos += 3;
			line.erase(0, pos);
		}
		else if(line.substr(pos-1, pos) == ",") { //list continues
			type = line.substr(0, pos - 1);
			convertType(type);
			pos += 1;
			line.erase(0, pos);
		}
		else if(line.substr(pos-1,pos) == ")") { //list ends
			type = line.substr(0, pos - 1);
			convertType(type);
			pos += 1;
			line.erase(0, pos);
			isDone = true;
		}
		//create and insert attribute
		attribute temp(name, type, false, size);
		attrVect.push_back(temp);
	}
	//Parse primary key
	size_t pos = line.find('(');
	if(pos == string::npos) {
		cout << "Error: failed to provide Primary Key\n";
		return;
	}
	pos += 1;
	line.erase(0, pos);
	isDone = false;
	while(!isDone) {
		string key;
		if((pos = line.find(',')) == string::npos) {
			pos = line.find(')');
			key = line.substr(0, pos);
			line.erase(0, pos);
			isDone = true;
		}
		else {
			key = line.substr(0, pos);
			pos += 2;
			line.erase(0, pos);
		}
		primaryKeys.push_back(key);
	}
	//Assign primary key
	for(int i = 0; i < primaryKeys.size(); ++i) {
		for(int j = 0; j < attrVect.size(); ++j) {
			if(attrVect[j].attributeName == primaryKeys[i]) {
				attrVect[j].isPk = true;
			}
		}
	}
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


