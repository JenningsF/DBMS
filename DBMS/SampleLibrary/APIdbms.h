//********************************************************
// Name			: 
// Date			: 13 Febuary 2015
// Subject		: CSCE 315-504
// Assignment	: Project 2: DBMS
// Updated		: 11 February 2015
// Description	: Database Engine where functions will be declared
//********************************************************
#ifndef APIDBMS_H
#define APIDBMS_H

#include "DBengine.h"

/*
	Parses input string
	Input: Reference to DB, String to be parsed
	Output: bool value for commands such as OPEN to signal a successful completion
*/
bool parse_db(DBengine &db, string parse_string);
/*
	Input: Reference to DB Engine, filename
	Output: returns the relation/table
*/
Relation* getTable_db(DBengine &db, string fromName);
/*
	Used to check whether a database exists
	Input: Reference to DB Engine, filename
*/
bool tableExists_db(DBengine &db, string tableName);
#endif
