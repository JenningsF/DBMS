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
					DB.create(query[i].viewName, query[i].attribs, query[i].attributes, query[i].fromName);
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
					DB.del(query[i].viewName, query[i].condition_one, query[i].condition_two, query[i].comparisons);
					break;
				case eShow: //good
					DB.show(query[i].viewName);
					break;
				case eSelect:
					if (i != 0) {
						query[i - 1].fromName = DB.select(query[i].fromName, query[i].attributes);
						query[i - 1].attribs = DB.getTableAttributes(query[i - 1].fromName);
						query[i - 1].attributes = DB.getTableKeys(query[i - 1].fromName);
					}
					break;
				case eUnion:
					if (i != 0) {
						query[i - 1].fromName = DB.relationUnion(query[i].viewName, query[i].fromName);
						query[i - 1].attribs = DB.getTableAttributes(query[i - 1].fromName);
						query[i - 1].attributes = DB.getTableKeys(query[i - 1].fromName);
					}
					break;
				case eDiff:
					if (i != 0) {
						query[i - 1].fromName = DB.relationDiff(query[i].viewName, query[i].fromName);
						query[i - 1].attribs = DB.getTableAttributes(query[i - 1].fromName);
						query[i - 1].attributes = DB.getTableKeys(query[i - 1].fromName);
					}
					break;
				case eCross:					
					if (i != 0) {
						query[i - 1].fromName = DB.relationCross(query[i].viewName, query[i].fromName);
						query[i - 1].attribs = DB.getTableAttributes(query[i - 1].fromName);
						query[i - 1].attributes = DB.getTableKeys(query[i - 1].fromName);
					}
					break;
				case eRename:

					break;
				case eProject:
					if (i != 0) {
						query[i - 1].fromName = DB.project(query[i].fromName, query[i].attributes);
						query[i - 1].attribs = DB.getTableAttributes(query[i - 1].fromName);
						query[i - 1].attributes = DB.getTableKeys(query[i - 1].fromName);
					}
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