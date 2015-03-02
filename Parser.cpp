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
//Parser:: parse_command()
void Parser::parse_command(){
	string temp = line.substr(0, line.find(' '));
	size_t delim = line.find(' ') + 1;
	line.erase(0, delim);
	string_command command = hashit(temp);
	switch (command){
	case eOpen: //good
		element open;
		open.command = eOpen;
		open.viewName = line.substr(0, line.find('\0'));
		query.push_back(open);
		break;
	case eUpdate:
		delim = line.find(' ') + 1;
		line.erase(0,delim);
		parser_update();
	case eCreate: //good
		delim = line.find(' ') + 1; //erase TABLE before calling parse_create
		line.erase(0, delim);
		parse_create();
		break;
	case eDelete:
		line.erase(0, line.find(" FROM ") + 6);
		parse_delete();
		break;
	case eInsert:
		cout << "Call insert for: " << line << endl;
		cin >> pause;
		line.erase(0, line.find("INTO ") + 5);
		parse_insert(line);
		break;
	case eShow: //good
		element show;
		show.command = eShow;
		show.viewName = line.substr(0, line.find('\0'));
		query.push_back(show);
		break;
	case eWrite: //good
		element write;
		write.command = eWrite;
		write.viewName = line.substr(0, line.find('\0'));
		query.push_back(write);
		break;
	case eClose: //good
		element close;
		close.command = eClose;
		close.viewName = line.substr(0, line.find('\0'));
		query.push_back(close);
		break;
	case eExit: //good
		break;
	default: //good
		printf("Error: unsupported command\n");
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
	//Temp Variables
	element create;
	string name, type;
	int size = 0;
	//Parser
	bool isDone = false;
	create.command = eCreate;
	while(!isDone) {
		size_t pos = line.find(' ');
		create.viewName = line.substr(0, pos);
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
			size = atoi(line.substr(0,pos));
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
		create.attribs.push_back(temp);
	}
	//Parse primary key
	size_t pos = line.find('(');
	if(pos == string::npos) {
		printf("Error: failed to provide Primary Key\n");
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
		create.attribss.push_back(key);
	}
	query.push_back(create);
}

void Parser::parse_update() {
	element update;
	update.command = eUpdate;
	size_t pos = line.find(' ');
	update.viewName = line.substr(0, pos);
	pos += 1;
	line.erase(0,pos);
	pos = line.find(' ') + 1;
	line.erase(0, pos);
	pos = line.find(' ');
	update.column = line.substr(0, pos);
	pos = line.find('\"') + 1;
	line.erase(0,pos);
	pos = line.find('\"');
	update.value = line.substr(0,pos)
	pos += 2;
	line.erase(0,pos)
	pos = line.find(' ') + 1;
	line.erase(0, pos);
	
	while(true)
	{
		string temp = "";
		string temp2 = "";
		pos = line.find('=') + 2;
		temp += line.substr(0,pos)
		line.erase(0,pos);
		pos = line.find(' ');
		if (pos == string::npos) {
			// No more conditions
			pos = line.find('\0');
			temp += line.substr(0, pos);
			update.attributes.push_back(temp);
			line.erase(0, pos);
			break;
		}
		else {
			// Additional conditions expected
			temp += line.substr(0,pos);
			update.attributes.push_back(temp);
			line.erase(0, pos+1);
			pos = line.find(" ");
			temp2 = line.substr(0, pos);
			update.attributes.push_back(temp2);
			line.erase(0, pos+1);
		}
	}
	query.push_back(update);
}

//--------------------------------------------------------------------------------------------------------------------
//Parser:: parse_insert()
//Example: INSERT INTO animals FROM ...
void Parser::parse_insert() {
	string expr, dataVal;
	vector<string> literals;
	element elem;	// holds critical data from parsed command
	// relation_name = line.substr(0, l.find(' '));
	elem.viewName = line.substr(0, l.find(' '));	// would put animals in viewName
	line.erase(0, line.find("FROM") + 5);
	expr = line.substr(0, line.find(" "));
	if (expr != "RELATION") {
		//need to get attributes of relation_name
		line.erase(0, line.find('(') + 1);	// Joe, cat, 4);
		line.erase(line.find(')'), line.find(';') + 1);
		line += " ";
		while (line.size() > 0) {
			dataVal = line.substr(0, line.find(','));
			elem.attributes.push_back(dataVal);
			line.erase(0, line.find(' ') + 1);
		}
	}
	else {	//needs to parse expr
		line.erase(0, line.find("RELATION ") + 9);
		expr = line.substr(0, line.find('('))
		if (expr == "project")
			// *Cody, please check if I'm calling this correctly*
			parse_query();
		else {
			dataVal = line.substr(0, line.find(';'));
			elem.fromName = dataVal;
		}
		// expr = line.substr(0, line.find(';'));
	}
}

//--------------------------------------------------------------------------------------------------------------------
//Parser:: parse_delete()
//Example: DELETE FROM animals WHERE
void Parser::parse_delete() {
	element del;
	del.command = eDelete;
	del.viewName = line.substr(0, line.find(' '));
	line.erase(0, line.find("WHERE ") + 6);
	// while (line.size() > 0) {
	// 	dataVal = line.substr(0, line.find(','));
	// 	insert.attributes.push_back(dataVal);
	// 	line.erase(0, line.find(' ') + 1);
	// }
	while(true) {
		string temp = "";
		string temp2 = "";
		pos = line.find('=') + 4;
		temp += line.substr(0,pos)
		line.erase(0,pos);
		pos = line.find(' ');
		if (pos == string::npos) {
			// No more conditions
			pos = line.find('\0');
			temp += line.substr(0, pos);
			update.attributes.push_back(temp);
			line.erase(0, pos);
			break;
		}
		else {
			// Additional conditions expected
			temp += line.substr(0,pos);
			update.attributes.push_back(temp);
			line.erase(0, pos+1);
			pos = line.find(" ");
			temp2 = line.substr(0, pos);
			update.attributes.push_back(temp2);
			line.erase(0, pos+1);
		}
	}
	query.push_back(del);
}

