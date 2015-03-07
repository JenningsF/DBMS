#ifndef RELATION_REGISTRY_H
#define RELATION_REGISTRY_H

/*****************************************
Includes
*****************************************/

#include <assert.h>
#include <map>
#include "handler.h"

using namespace std;

/**
   -This class defines a registry that tracks and stores database tables.
*/
class RelationsRegistry{
   map<string, Table> table_register;   //Map of stored tables indexed by name.

public:
   /*
      -Returns a copy of the Table with name tableName.
      -Arguments: Name of the table to return.
      -Returns: A copy of the specified table.
   */
   Table getTable(string tableName);

   /*
      -Returns a reference of the Table with name tableName.
      -Arguments: Name of the table to return.
      -Returns: A reference of the specified table.
   */
   Table& getTableReference(string tableName);

   /*
      -Adds a Table with the given tableName to the registry.
      -Arguments: Name of the table to add, the Table to add.
   */
   void addTable(string tableName, Table tab);

   /*
      -Removes a Table with the given tableName from the registry.
      -Arguments: Name of the table to remove.
   */
   void removeTable(string tableName); 

   /*
      -Returns whether a Table with name tableName is in the registry.
      -Arguments: Name of the table to check.
      -Returns: Whether the Table exists.
   */
   bool hasTable(string tableName);

   /*
      -Returns the names of all tables in the registry.
      -Returns: Vector of Table names.
   */
   vector<string> getAllTableNames();

};

#endif

