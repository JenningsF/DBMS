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

void Parser:: parse(string l){
	string temp = l.substr(0, l.find(' '));
	if (temp == "CREATE" || temp == "OPEN" || temp == "INSERT" || temp == "SHOW" || temp == "WRITE" || temp == "CLOSE" || temp == "EXIT")
		parse_command(l);
	else parse_query();
}


void Parser:: parse_command(string l){
	string temp = l.substr(0, l.find(' '));
	l.erase(0, l.find(' ') + 1);
	string relation_name, pause;
	string_command command = hashit(temp);
	switch (command){
	case eCreate:
		cout << "Call create for: " << l << endl;
		cin >> pause;
		break;
	case eInsert:
		break;
	case eShow:
		relation_name = line.substr(0, line.find(' '));
		cout << "Call show for relation: " << relation_name << endl;
		break;
	case eWrite:
		relation_name = line.substr(0, line.find(' '));
		cout << "Call write to table name: " << relation_name << endl;
		cin >> pause;
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

int main(){


	string test_command = "CREATE TABLE animals (name VARCHAR(20), kind VARCHAR(8), years INTEGER) PRIMARY KEY (name, kind);";
	Parser test(test_command);
	
	


}