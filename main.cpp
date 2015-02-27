//********************************************************
// Name			: 
// Date			: 13 Febuary 2015
// Subject		: CSCE 315-504
// Assignment	: Project 2: DBMS
// Updated		: 11 February 2015
// Description	: Database Engine where functions will be declared
//********************************************************

int main(int argc, char const *argv[]) {
	DBengine DB;
	Parser parse;
	string temp;
	while (cin.getline(temp)) {
		parse.parse(temp);
		switch() {
			case eOpen:
				DB.open(parse.getFromName());
			case eClose:
				DB.close(parse.getFromName());
			case eInsert:
				DB.insert(parse.getTableName(), parse.getRowData());
			case eCreate:
				DB.create(parse.getFromName(), parse.getAttributes(), parse.getPrimaryKeys());
			case eWrite:
				DB.write(parse.getFromName());
			case eExit:
				DB.exitEngine();
			case eUpdate:
				DB.update(parse.getTableName(), parse.getRowIndex(), parse.getColName(), parse.getData());
			case eDelete:
				DB.del(parse.getTableName(), parse.getColName(), parse.getWhatToDel());
			case eShow:
				Db.show(parse.getTableName());
		}
		// if (parse.whatComm == eCreate)
		// 	DB.create(parse.getFromName());
		// else if (parse.isShow)
		// 	DB.show(parse.tableName());
	}
}