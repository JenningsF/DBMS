#ifndef TABLE_H
#define TABLE_H

/*****************************************
Includes
*****************************************/

#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;

/*****************************************
Utilities
*****************************************/

enum dataType { Integer = 1, Double, Varchar };

class BadType {};

/*****************************************
Structure Definitions
*****************************************/

/*
   -This struct defines an Attribute for a Table.
   -Contains the attribute name, type, and whether it is part of the primary
    key.
*/
struct Attribute {
   string name;
   dataType type;
   bool key;

   Attribute(string attName, dataType attType, bool attKey = false )
      : name(attName), type(attType), key(attKey) {}
};

/**
   -This class defines a Table that stores a vector of tuples (vector of
    strings) that conform to the Table's Attributes.
*/
class Table{

   string name;
   vector<Attribute> attributeList;   //List of all Attributes.
   vector<vector<string> > tableData;   //Vector matrix of Table data.
   map<vector<string>, char> pKeys;   //Contains all primary key values.
   bool hasPkey;  //Indicates whether the table has primary keys.

public:
   /* Returns the name of the table */
   string getTableName(){return name;}
   
   /* Removes a tuple at the index specified. Returns true if operation 
      succeeded, false otherwise */
   bool removeTuple(int index);
   
   /* Adds the vector as tuple to the Table. Returns true if operation
      succeeded, false otherwise */
   bool addTuple(vector<string>);
   
   /* Returns the Table index of the tuple identified by the vector */
   int getTupleIndex(vector<string>);
   
   /* Returns the tuple as vector at the given index */
   vector<string> getTuple(int index);
   
   /* Updates the tuple at the given index, replacing the attribute named
      oldVal with the value newVal */
   bool updateTuple(int index, string oldVal, string newVal);
   
   /* Adds the given attribute to the Table. */
   bool addAttribute(Attribute);
   
   /* Returns the index of the attribute with attributeName */
   int getAttributeIndex(string attributeName);
   
   /* Removes the attribute at the given index */
   bool removeAttribute(int index);
   
   /* Returns the Attribute at the given index */
   Attribute getAttribute(int index);
   
   /* Returns the number of attributes in the table */
   int numberOfAttributes(){return attributeList.size();}
   
   /* Returns a vector of Attributes containing all the table's attributes */
   vector<Attribute> getAttributeList(){return attributeList;}
   
   /* Returns the type (int ID from dataType enum) of attribute named 
      attributeName */
   int getAttributeType(string attributeName);
   
   /* Renames the attribute at index with the name rename */
   bool renameAttribute(int index, string rename){
      attributeList[index].name = rename;
      return true;
   }

   /* Prints the table. This is used for the SHOW command */
   bool printTable();
   
   /* Constructors */
   Table(string tableName, vector<Attribute> attributes);
   Table() {hasPkey = false;}
   
   /* Returns the number of tuples in the table */
   int size() {return tableData.size();}
   
   /* Returns the individual value in the tuple indexed by tupleIndex at
      position attrIndex in the tuple */
   string getValue(int attrIndex, int tupleIndex){
      return tableData[tupleIndex][attrIndex];
   }

   /* Converts the vector of attributes to the format of the table's primary
      key vector */
   vector<string> toPkey(vector<string>);
};


/*****************************************
Table-specific Utilities
*****************************************/

#endif
