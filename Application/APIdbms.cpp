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

APIdbms::APIdbms(DBengine *db) {
	DB = db;
}

bool APIdbms::parse_db(string parse_string) {
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
				return DB->open(query[i].viewName);
				break;
			/*
				Writes the file
				Closes the database file
				Removes the relation from the engine
				Input: filename
			*/
			case eClose: //good
				return DB->close(query[i].viewName);
				break;
			case eInsert:
				DB->insert(query[i].viewName, query[i].attributes);
				break;
			case eCreate: //good
				DB->create(query[i].viewName, query[i].tableAttribs, query[i].tableAttributes, query[i].fromName);
				break;
			case eWrite: //good
				DB->write(query[i].viewName);
				break;
			/*
				Exits the DB engine
				Does not write any open database
			*/
			case eExit: //good
				DB->exitEngine();
				return;
			case eUpdate:
				DB->update(query[i].viewName, query[i].column, query[i].value,
							query[i].condition_one, query[i].condition_two, query[i].comparisons);
				break;
			case eDelete:
				DB->del(query[i].viewName, query[i].condition_one, query[i].condition_two, query[i].comparisons);
				break;
			/*
				Displays the contents of a table
				Input: filename
			*/
			case eShow: //good
				DB->show(query[i].viewName);
				break;
			case eSelect:
				if (i != 0) {
					query[i - 1].fromName = DB->select(query[i].fromName, query[i].attributes);
					query[i - 1].tableAttribs = DB->getTableAttributes(query[i - 1].fromName);
					query[i - 1].tableAttributes = DB->getTableKeys(query[i - 1].fromName);
				}
				break;
			case eUnion:
				if (i != 0) {
					query[i - 1].fromName = DB->relationUnion(query[i].viewName, query[i].fromName);
					query[i - 1].tableAttribs = DB->getTableAttributes(query[i - 1].fromName);
					query[i - 1].tableAttributes = DB->getTableKeys(query[i - 1].fromName);
				}
				break;
			case eDiff:
				if (i != 0) {
					query[i - 1].fromName = DB->relationDiff(query[i].viewName, query[i].fromName);
					query[i - 1].tableAttribs = DB->getTableAttributes(query[i - 1].fromName);
					query[i - 1].tableAttributes = DB->getTableKeys(query[i - 1].fromName);
				}
				break;
			case eCross:					
				if (i != 0) {
					query[i - 1].fromName = DB->relationCross(query[i].viewName, query[i].fromName);
					query[i - 1].tableAttribs = DB->getTableAttributes(query[i - 1].fromName);
					query[i - 1].tableAttributes = DB->getTableKeys(query[i - 1].fromName);
				}
				break;
			case eRename:
				if (i != 0) {
					query[i - 1].fromName = DB->rename(query[i].fromName, query[i].attributes);
					query[i - 1].tableAttribs = DB->getTableAttributes(query[i - 1].fromName);
					query[i - 1].tableAttributes = DB->getTableKeys(query[i - 1].fromName);
				}
				break;
			case eProject:
				if (i != 0) {
					query[i - 1].fromName = DB->project(query[i].fromName, query[i].attributes);
					query[i - 1].tableAttribs = DB->getTableAttributes(query[i - 1].fromName);
					query[i - 1].tableAttributes = DB->getTableKeys(query[i - 1].fromName);
				}
				break;
			case ERROR:
			default:
				cout << "\nINVALID COMMAND" << endl;
				return;
				break;
		}
	}
	parse.cleanQuery();
	return true;
}

Relation* APIdbms::getTable_db(string fromName) {
	return DB->getTable(fromName);
}

bool APIdbms::tableExists_db(string tableName) {
	return DB->tableExists(tableName);
}