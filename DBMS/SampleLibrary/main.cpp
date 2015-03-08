//********************************************************
// Name			: 
// Date			: 13 Febuary 2015
// Subject		: CSCE 315-504
// Assignment	: Project 2: DBMS
// Updated		: 11 February 2015
// Description	: Database Engine where functions will be declared
//********************************************************
#include "APIdbms.h"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
	DBengine DB;
	parse_db(DB, "CREATE TABLE animals (name VARCHAR(20), kind VARCHAR(8), years INTEGER) PRIMARY KEY (name, kind);");
	cout << "HELO" << endl;
	parse_db(DB, "INSERT INTO animals VALUES FROM (\"Joe\", \"cat\", 24);");
	cout << "one" <<endl;
	if (tableExists_db(DB, "animals")) {
		cout << "two" << endl;
		Relation* temp_table = getTable_db(DB, "animals");
		string column = "name";
		cout << temp_table->getRow(0).get(column) << endl;
	}
	else cout << "NOPE";
	return 0;
}