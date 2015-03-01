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

using namespace std;

int main(int argc, char const *argv[]) {
	DBengine DB;
	Parser parse;
	string temp;
	while (getline(cin, temp)) {
		parse.parse(temp);
		switch(parse.getCommand()) {
			case eOpen:
				DB.open(parse.getFromName());
			case eClose:
				DB.close(parse.getFromName());
			case eInsert:
		//		DB.insert(parse.getViewName(), parse.getRowData());
			case eCreate:
		//		DB.create(parse.getFromName(), parse.getAttributes(), parse.getPrimaryKeys());
			case eWrite:
				DB.write(parse.getFromName());
			case eExit:
				DB.exitEngine();
			case eUpdate:
		//		DB.update(parse.getViewName(), parse.getRowIndex(), parse.getColName(), parse.getData());
			case eDelete:
		//		DB.del(parse.getViewName(), parse.getColName(), parse.getWhatToDel());
			case eShow:
				DB.show(parse.getViewName());
		}
		// if (parse.whatComm == eCreate)
		// 	DB.create(parse.getFromName());
		// else if (parse.isShow)
		// 	DB.show(parse.tableName());
	}
}