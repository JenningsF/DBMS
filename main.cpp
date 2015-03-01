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
			case eOpen: //good
				DB.open(parse.getFromName());
			case eClose: //good
				DB.close(parse.getFromName());
			case eInsert:
		//		DB.insert(parse.getViewName(), parse.getRowData());
			case eCreate: //good
				DB.create(parse.getTableName(), parse.getAttributes(), parse.getPrimaryKeys());
			case eWrite: //good
				DB.write(parse.getFromName());
			case eExit: //good
				DB.exitEngine();
			case eUpdate:
		//		DB.update(parse.getViewName(), parse.getRowIndex(), parse.getColName(), parse.getData());
			case eDelete:
		//		DB.del(parse.getViewName(), parse.getColName(), parse.getWhatToDel());
			case eShow: //good
				DB.show(parse.getViewName());
		}
		// if (parse.whatComm == eCreate)
		// 	DB.create(parse.getFromName());
		// else if (parse.isShow)
		// 	DB.show(parse.tableName());
	}
}