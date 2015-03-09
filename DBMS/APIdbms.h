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
	Input: Reference to DB, String
	Output: None
*/
bool parse_db(DBengine &db, string parse_string);
/*
	Input: filename
	Output: returns the relation/table
*/
Relation* getTable_db(DBengine &db, string fromName);
/*
	Used to check whether a database exists
	Input: filename
*/
bool tableExists_db(DBengine &db, string tableName);
#endif
