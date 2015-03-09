//********************************************************
// Name			: 
// Date			: 13 Febuary 2015
// Subject		: CSCE 315-504
// Assignment	: Project 2: DBMS
// Updated		: 11 February 2015
// Description	: Database Engine where functions will be declared
//********************************************************
#include "DBengine.h"
#include "Parser.h"
#include <iostream>
#include <sstream>
#include "APIdbms.h"

bool parse_db(DBengine &db, string parse_string) {
	Parser parse;
	parse.parse(parse_string);
	vector<element> query = parse.getQuery();
	int a = 2;
	for (int i = query.size() - 1; i >= 0; --i) {
		switch (query[i].command) {
			/*
				Opens a DB and loads the table into the engine
				Input: filename
				Output: true if table is opened
						false if table does not open
			*/
			case eOpen: //good
				return db.open(query[i].viewName);
				break;
			/*
				Writes the file
				Closes the database file
				Removes the relation from the engine
				Input: filename
			*/
			case eClose: //good
				return db.close(query[i].viewName);
				break;
			case eInsert:
				db.insert(query[i].viewName, query[i].attributes);
				break;
			case eCreate: //good
				db.create(query[i].viewName, query[i].tableAttribs, query[i].tableAttributes, query[i].fromName);
				break;
			case eWrite: //good
				db.write(query[i].viewName);
				break;
			/*
				Exits the DB engine
				Does not write any open database
			*/
			case eExit: //good
				db.exitEngine();
				return true;
			case eUpdate:
				db.update(query[i].viewName, query[i].column, query[i].value,
							query[i].condition_one, query[i].condition_two, query[i].comparisons);
				break;
			case eDelete:
				db.del(query[i].viewName, query[i].condition_one, query[i].condition_two, query[i].comparisons);
				break;
			/*
				Displays the contents of a table
				Input: filename
			*/
			case eShow: //good
				db.show(query[i].viewName);
				break;
			case eSelect:
				if (i != 0) {
					query[i - 1].fromName = db.select(query[i].fromName, query[i].attributes);
					query[i - 1].tableAttribs = db.getTableAttributes(query[i - 1].fromName);
					query[i - 1].tableAttributes = db.getTableKeys(query[i - 1].fromName);
				}
				break;
			case eUnion:
				if (i != 0) {
					query[i - 1].fromName = db.relationUnion(query[i].viewName, query[i].fromName);
					query[i - 1].tableAttribs = db.getTableAttributes(query[i - 1].fromName);
					query[i - 1].tableAttributes = db.getTableKeys(query[i - 1].fromName);
				}
				break;
			case eDiff:
				if (i != 0) {
					query[i - 1].fromName = db.relationDiff(query[i].viewName, query[i].fromName);
					query[i - 1].tableAttribs = db.getTableAttributes(query[i - 1].fromName);
					query[i - 1].tableAttributes = db.getTableKeys(query[i - 1].fromName);
				}
				break;
			case eCross:					
				if (i != 0) {
					query[i - 1].fromName = db.relationCross(query[i].viewName, query[i].fromName);
					query[i - 1].tableAttribs = db.getTableAttributes(query[i - 1].fromName);
					query[i - 1].tableAttributes = db.getTableKeys(query[i - 1].fromName);
				}
				break;
			case eRename:
				if (i != 0) {
					query[i - 1].fromName = db.rename(query[i].fromName, query[i].attributes);
					query[i - 1].tableAttribs = db.getTableAttributes(query[i - 1].fromName);
					query[i - 1].tableAttributes = db.getTableKeys(query[i - 1].fromName);
				}
				break;
			case eProject:
				if (i != 0) {
					query[i - 1].fromName = db.project(query[i].fromName, query[i].attributes);
					query[i - 1].tableAttribs = db.getTableAttributes(query[i - 1].fromName);
					query[i - 1].tableAttributes = db.getTableKeys(query[i - 1].fromName);
				}
				break;
			case ERROR:
			default:
				cout << "\nINVALID COMMAND" << endl;
				return true;
				break;
		}
	}
	parse.cleanQuery();
	return true;
}

Relation* getTable_db(DBengine &db, string fromName) {
	return db.getTable(fromName);
}

bool tableExists_db(DBengine &db, string tableName) {
	return db.tableExists(tableName);
}