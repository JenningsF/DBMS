/*****************************************
Includes
*****************************************/

#include "RelationsRegistry.h"

using namespace std;

/*
     -Returns a copy of the Table with name tableName.
     -Arguments: Name of the table to return.
     -Returns: A copy of the specified table.
*/
Table RelationsRegistry::getTable(string tableName) {
	if (!hasTable(tableName)) {
		throw NoSuchTable();
	 }
      map<string, Table>::iterator it = table_register.find(tableName);
      return it->second;
}

/*
	-Returns a reference of the Table with name tableName.
	-Arguments: Name of the table to return.
	-Returns: A reference of the specified table.
*/
Table& RelationsRegistry::getTableReference(string tableName) {
  if (!hasTable(tableName)) {
		throw NoSuchTable();
  }
	map<string, Table>::iterator it = table_register.find(tableName);
	return it->second;
}

/*
	-Adds a Table with the given tableName to the registry.
	-Arguments: Name of the table to add, the Table to add.
*/
void RelationsRegistry::addTable(string tableName, Table tab) {
	table_register.insert(pair<string, Table>(tableName, tab));
}

/*
	-Removes a Table with the given tableName from the registry.
	-Arguments: Name of the table to remove.
*/
void RelationsRegistry::removeTable(string tableName) {
	table_register.erase(table_register.find(tableName));
}

/*
	-Returns whether a Table with name tableName is in the registry.
	-Arguments: Name of the table to check.
	-Returns: Whether the Table exists.
*/
bool RelationsRegistry::hasTable(string tableName) {
	return (table_register.find(tableName) != table_register.end());
}

/*
	-Returns the names of all tables in the registry.
	-Returns: Vector of Table names.
*/
vector<string> RelationsRegistry::getAllTableNames() {
	vector<string> name_vec;
	map<string, Table>::iterator it = table_register.begin();
	for (it; it!=table_register.end(); ++it) {
		name_vec.push_back(it->first);
	}

	return name_vec;
}



