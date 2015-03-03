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
		vector<element> query = parse.getQuery();
		int a = 2;
		for (int i = query.size() - 1; i >= 0; --i) {
			switch (query[i].command) {
				case eOpen: //good
					DB.open(query[i].viewName);
					break;
				case eClose: //good
					DB.close(query[i].viewName);
					break;
				case eInsert:
					DB.insert(query[i].viewName, query[i].attributes);
					break;
				case eCreate: //good
					DB.create(query[i].viewName, query[i].attribs, query[i].attributes);
					break;
				case eWrite: //good
					DB.write(query[i].viewName);
					break;
				case eExit: //good
					DB.exitEngine();
					return 0;
				case eUpdate:
					//		DB.update(parse.getViewName(), parse.getRowIndex(), parse.getColName(), parse.getData());
					break;
				case eDelete:
					//		DB.del(parse.getViewName(), parse.getColName(), parse.getWhatToDel());
					break;
				case eShow: //good
					DB.show(query[i].viewName);
					break;
				case eSelect:
					break;
				case eUnion:
					break;
				case eDiff:
					break;
				case eCross:
					break;
				case eRename:
					break;
				case eProject:
					break;
				case ERROR:
				default:
					cout << "\nINVALID COMMAND" << endl;
					return -1;
					break;
			}
		}
		parse.cleanQuery();
	}
}