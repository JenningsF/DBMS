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
//------------------------------------------------------------------------------------------------------
//Parser:: parse(string l)

void Parser::parse(string l){
	line = l;
	string temp = line.substr(0, line.find(' '));
	if (temp == "CREATE" || temp == "OPEN" || temp == "INSERT" || temp == "SHOW" || temp == "WRITE" || temp == "CLOSE" || temp == "UPDATE" || temp == "EXIT" || temp == "DELETE")
		parse_command();
	else {
		//Strip off viewName
		size_t pos = 0;
		element create;
		pos = line.find(" ");
		create.viewName = line.substr(0, pos);
		create.command = eCreate;
		line.erase(0, pos + 4);
		query.push_back(create);
		parse_query();
	}
	element last = query[query.size() - 1];
	if (last.command == eCreate || last.command == eWrite || last.command == eShow || last.command == eClose || last.command == eOpen || last.command == eInsert) {
		if (last.viewName == "") query[query.size() - 1].command = ERROR;
	}
	else if (last.command == eCross || last.command == eUnion || last.command == eDiff) {
		if (last.viewName == "" || last.fromName == "") query[query.size() - 1].command = ERROR;
	}
	else if (last.command == eExit) {}
	else {
		if (last.fromName == "") query[query.size() - 1].command = ERROR;
	}
}

//--------------------------------------------------------------------------------------------------------
//Parser:: parse_command()
void Parser::parse_command(){
	element open;
	element show;
	element write;
	element close;
	element exit;
	char pause;
	string temp = line.substr(0, line.find(' '));
	size_t delim = line.find(' ') + 1;
	line.erase(0, delim);
	string_command command = hashit(temp);
	switch (command){
	case eOpen: //good
		open.command = eOpen;
		open.viewName = line.substr(0, line.find(';'));
		query.push_back(open);
		break;
	case eUpdate:
		parse_update();
		break;
	case eCreate: //good
		delim = line.find(' ') + 1; //erase TABLE before calling parse_create
		line.erase(0, delim);
		parse_create();
		break;
	case eInsert:
		line.erase(0, line.find("INTO ") + 5);
		parse_insert();
		break;
	case eShow: //good
		show.command = eShow;
		show.viewName = line.substr(0, line.find(';'));
		line.erase(0, line.find(';'));
		query.push_back(show);
		break;
	case eWrite: //good
		write.command = eWrite;
		write.viewName = line.substr(0, line.find(';'));
		line.erase(0, line.find(';'));
		query.push_back(write);
		break;
	case eClose: //good
		close.command = eClose;
		close.viewName = line.substr(0, line.find(';'));
		line.erase(0, line.find(';'));
		query.push_back(close);
		break;
	case eDelete:
		line.erase(0, line.find("FROM") + 5);
		parse_delete();
		break;
	case eExit: //good
		exit.command = eExit;
		query.push_back(exit);
		break;
	default: //good
		cout << "Error: unsupported command\n";
		break;
	}
}

//-------------------------------------------------------------------------------------------------------
//void parse_type(string l, vector<attribute> attrVector, vector<string> primaryKey)

//--------------------------------------------------------------------------------------------------------
//Parser::parse_query
void trimQuote(string& s) {
	if (s[0] == '"' || s[0] == '\'' || s[0] == ' ') {
		s = s.substr(1, s.length());
	}
	if (s[s.length() - 1] == '"' || s[s.length() - 1] == '\'' || s[s.length() - 1] == ' ') {
		s = s.substr(0, s.length() - 1);
	}
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
	size_t pos_or = 0;
	size_t pos_and = 0;
	pos = select_string.find(" ");
	type = select_string.substr(0, pos);
	select_string.erase(0, pos + 2);
	// While loop to parse all conditions of select
	while (select_string.length() != 0) {
		element select;
		// Get column name and erase from command
		select.query_type = type;
		select.command = eSelect;
		pos_or = select_string.find("||");
		pos_and = select_string.find("&&");
		pos = select_string.find(")");
		if (pos_or < pos_and && pos_or < pos) {
			select.attributes.push_back(select_string.substr(0, pos_or));
			select_string.erase(0, pos_or+2);
		}
		else if (pos_and < pos_or && pos_and < pos) {
			select.attributes.push_back(select_string.substr(0, pos_and));
			select_string.erase(0, pos_and+2);
		}
		else {
			select.attributes.push_back(select_string.substr(0, pos));
			select_string.erase(0, pos + 2);
		}
		//trimQuote(select.value);
		query.push_back(select);
	}
	return ValidateSelect();
}

//Parses rename and project
vector<string> Parser::parse_list(string list_string) {
	vector<string> list;
	size_t pos = 0;
	while (list_string[0] == ' ' || list_string[0] == '(') list_string.erase(0, 1);
	while (true){
		pos = list_string.find(',');
		if (pos == string::npos){
			pos = list_string.find(')');
			string temp = list_string.substr(0, pos);
			list.push_back(temp);
			break;
		}
		else {
			string temp = list_string.substr(0, pos);
			list.push_back(temp);
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
	size_t pos = min(line.find(' '), line.find(';'));

	//Process Query
	while(pos != string::npos){
		if (line == ");") return;
		while (line[0] == ' ' || line[0] == '(') line.erase(0, 1);
		pos = line.find(" ");
		string temp = line.substr(0,pos);
		if (temp == "select"){
			pos = line.find(')') + 1;
			string select_string = line.substr(0, pos);
			bool test = parse_select(select_string);
			line.erase(0, pos + 1);
		}
		else if (temp == "rename" || temp == "project"){
			line.erase(0, pos + 1);
			pos = line.find(')') + 1;
			element rename_project;
			if (temp == "rename") rename_project.command = eRename;
			else rename_project.command = eProject;
			string list_string = line.substr(0, pos);
			rename_project.attributes = parse_list(list_string);
			query.push_back(rename_project);
			line.erase(0, pos);
		}
		else { // Relational algebra
			size_t pos_union = line.find('+');
			size_t pos_diff = line.find('-');
			size_t pos_cross = line.find('*');
			size_t pos_end = min(line.find(';'), line.find(')'));			
			size_t minimum = min(pos_union, pos_diff);
			minimum = min(minimum, pos_cross);
			// Check if end of query has been found
			if (pos_end < pos && pos_end < pos_union && pos_end < pos_diff && pos_end < pos_cross) {
				query[query.size() - 1].fromName = line.substr(0, pos_end);
				line.erase(0, pos_end + 1);
			}
			// Check if an algebra expression exists
			else if (minimum < pos_end) {
				element algebra;
				algebra.viewName = line.substr(0, line.find(' '));
				if (line[minimum] == '+') algebra.command = eUnion;
				else if (line[minimum] == '-') algebra.command = eDiff;
				else if (line[minimum] == '*') algebra.command = eCross;
				line.erase(0, minimum + 2);
				pos_end = min(line.find(';'), line.find(')'));
				algebra.fromName = line.substr(0, pos_end);
				line.erase(0, pos_end);
				query.push_back(algebra);
			}
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
	size_t pos = line.find(' ');
	create.viewName = line.substr(0, pos);
	line.erase(0, pos + 2); //erase tableName from line and first '('
	while (!isDone) {
		pos = line.find(' ');
		name = line.substr(0, pos); //attribute name
		line.erase(0, pos + 1);
		pos = line.find(' ');
		if (line.substr(pos - 2, 1) == ")") {//case for when there is a specified sized
			pos = line.find('(');
			type = line.substr(0, pos);
			convertType(type);
			line.erase(0, pos + 1);
			pos = line.find(')');
			size = atoi(line.substr(0, pos).c_str());
			pos += 3;
			line.erase(0, pos);
		}
		else if (line.substr(pos - 1, 1) == ",") { //list continues
			type = line.substr(0, pos - 1);
			convertType(type);
			pos += 1;
			line.erase(0, pos);
		}
		else if (line.substr(pos - 1, 1) == ")") { //list ends
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
	pos = line.find('(');
	if (pos == string::npos) {
		cout << "Error: failed to provide Primary Key\n";
		return;
	}
	pos += 1;
	line.erase(0, pos);
	isDone = false;
	while (!isDone) {
		string key;
		if ((pos = line.find(',')) == string::npos) {
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
		create.attributes.push_back(key);
	}
	query.push_back(create);
}

//UPDATE animals SET kind = "dog" WHERE name == "Joe" && sodf == "asdf"
//       viewName    column  value      c1        c2  comp
void Parser::parse_update() {
	element update;
	update.command = eUpdate;
	size_t pos = line.find(' ');
	update.viewName = line.substr(0, pos);
	pos += 1;
	line.erase(0,pos);
	line.erase(0,line.find(' ') + 1);
	while(true)
	{
		pos = line.find(' ');
		size_t pos2 = line.find("WHERE");
		if(pos2 < pos)
		{
			pos = line.find(' ') + 1;
			line.erase(0,pos);
			break;
		}
		update.column.push_back(line.substr(0, pos));
		pos = line.find('\"') + 1;
		line.erase(0, pos);
		pos = line.find('\"');
		update.value.push_back(line.substr(0, pos));
		pos += 2;
		line.erase(0, pos);
	}
	//WHERE something == LSLS
	line.erase(0,line.find(' ') + 1);
	while(true)
	{
		pos = line.find(' ');
		update.condition_one.push_back(line.substr(0,pos));
		pos = line.find('\"') + 1;
		line.erase(0, pos);
		pos = line.find('\"');
		update.condition_two.push_back(line.substr(0, pos));
		line.erase(0, pos + 2);
		pos = line.find(' ');
		if (pos == string::npos) {
			pos = line.find('\0');
			line.erase(0,pos);
			break;
		}
		else {
			update.comparisons.push_back(line.substr(0, pos));
			line.erase(0, pos + 1);
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
	elem.command = eInsert;
	elem.viewName = line.substr(0, line.find(' '));	// would put animals in viewName
	line.erase(0, line.find("FROM") + 5);
	expr = line.substr(0, line.find(" "));
	if (expr != "RELATION") {
		//need to get attributes of relation_name
		line.erase(0, line.find('(') + 1);	// Joe, cat, 4);
		line.erase(line.find(')'), line.find(';') + 1);
		line += " ";
		while (line.size() > 0) {
			size_t p = line.find(',');
			dataVal = line.substr(0, line.find(','));
			trimQuote(dataVal);
			elem.attributes.push_back(dataVal);
			line.erase(0, line.find(' ') + 1);
		}
	}
	else {	//needs to parse expr
		line.erase(0, line.find("RELATION ") + 9);
		expr = line.substr(0, line.find('('));
		parse_query();
	}
	query.push_back(elem);
}

//--------------------------------------------------------------------------------------------------------------------
//Parser:: parse_delete()
//Example: DELETE FROM animals WHERE
void Parser::parse_delete() {
	size_t pos;
	element del;
	del.command = eDelete;
	del.viewName = line.substr(0, line.find(' '));
	line.erase(0, line.find("WHERE ") + 6);
	while(true)
	{
		pos = line.find(' ');
		del.condition_one.push_back(line.substr(0,pos));
		pos = line.find('\"') + 1;
		line.erase(0, pos);
		pos = line.find('\"');
		del.condition_two.push_back(line.substr(0, pos));
		line.erase(0, pos + 2);
		pos = line.find(' ');
		if (pos == string::npos) {
			pos = line.find('\0');
			line.erase(0,pos);
			break;
		}
		else {
			del.comparisons.push_back(line.substr(0, pos));
			line.erase(0, pos + 1);
		}
	}
	query.push_back(del);
}

