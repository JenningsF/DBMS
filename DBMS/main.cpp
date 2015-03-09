//********************************************************
// Name			: 
// Date			: 13 Febuary 2015
// Subject		: CSCE 315-504
// Assignment	: Project 2: DBMS
// Updated		: 11 February 2015
// Description	: Database Engine where functions will be declared
//********************************************************
#include "DBengine.h"
#include <string>
#include "APIdbms.h"

using namespace std;

int main(int argc, char const *argv[]) {
	DBengine DB;
	string temp;
	while (getline(cin, temp)) {
		parse_db(DB, temp);
	}
}