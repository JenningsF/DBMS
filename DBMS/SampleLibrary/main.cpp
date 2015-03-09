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
	// Command to create table animals with parameters
	parse_db(DB, "CREATE TABLE animals (name VARCHAR(20), kind VARCHAR(8), years INTEGER) PRIMARY KEY (name, kind);");
	// Insert row into table animals
	parse_db(DB, "INSERT INTO animals VALUES FROM (\"Joe\", \"cat\", 24);");
	// Check if the DBengine has a table animals
	if (tableExists_db(DB, "animals")) {
		// Relation is our user defined class for table
		// Gets a workable class containing table data animals.
		Relation* temp_table = getTable_db(DB, "animals");
		// Specify column name for data you need
		string column = "name";
		// temp_table->getRow(0) returns first row in table 
		// .get(column) returns data from specified column/row
		cout << temp_table->getRow(0).get(column) << endl;
		// You can change data directly in table! How simple!
		temp_table->getRow(0).set("name", "Cody");
		cout << endl << temp_table->getRow(0).get(column) << endl;
		// Just make sure to call WRITE on the table!
		// This creates the db file!
		parse_db(DB, "WRITE animals;");
		parse_db(DB, "CLOSE animals;");
	}
	else cout << "NOPE";
	return 0;
}