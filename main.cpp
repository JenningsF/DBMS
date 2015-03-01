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
				break;
			case eClose:
				DB.close(parse.getFromName());
				break;
			case eInsert:
		//		DB.insert(parse.getViewName(), parse.getRowData());
				break;
			case eCreate:
		//		DB.create(parse.getFromName(), parse.getAttributes(), parse.getPrimaryKeys());
				break;
			case eWrite:
				DB.write(parse.getFromName());
				break;
			case eExit:
				DB.exitEngine();
				break;
			case eUpdate:
		//		DB.update(parse.getViewName(), parse.getRowIndex(), parse.getColName(), parse.getData());
				break;
			case eDelete:
		//		DB.del(parse.getViewName(), parse.getColName(), parse.getWhatToDel());
				break;
			case eShow:
				DB.show(parse.getViewName());
				break;
			default:
				break;
		}
		// if (parse.whatComm == eCreate)
		// 	DB.create(parse.getFromName());
		// else if (parse.isShow)
		// 	DB.show(parse.tableName());
	}
}