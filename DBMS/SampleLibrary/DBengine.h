//********************************************************
// Name			: 
// Date			: 13 Febuary 2015
// Subject		: CSCE 315-504
// Assignment	: Project 2: DBMS
// Updated		: 11 February 2015
// Description	: Database Engine where functions will be declared
//********************************************************

#ifndef DBEngine_H
#define DBEngine_H 1
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include "Attributes.h"

using namespace std;

class Row;

// Relation class to hold loaded table
class Relation {
	private:
		friend class Row;
		string name;
		int size;
		int colSize;
		vector<Row> rows;
		vector<string> columnNames;
	public:
		// What makes-up PrimaryKey 
		vector<string> keyParameters;
		// Column attributes
		vector<attribute> attributes;
		// Constructors
		Relation();
		Relation(string n, vector<attribute> attribs);
		Relation(string tableName, Relation* tab);
		// Getter functions
		string getName();
		int getSize();
		int getColumnSize();
		vector<string> getColumnNames();
		vector<Row> getRows();
		Row& getRow(int i);
		// Setter functions
		void setName(string n);
		void setAttributes(vector<attribute> attribs);
		void setKeyParameters(vector<string> keys);
		void setRows(vector<Row> r);
		void setColumnNames();
		// Add row to table
		void addRow(string primary);
		void addRow(Row r);
		void addRows(vector<Row> r);
		void incrementSize();
		// Add column to table
		void addAttribute(attribute attrib);
		// Remove row based on index
		void deleteRow(int i);
		// Overloaded cross product operator for relation
		Relation* operator*(Relation& rtable);
		Relation* operator+(Relation& rtable);
		Relation* operator-(Relation& rtable);
};

// Row class to make up relation
class Row {
	private:
		friend class Relation;
		string PK;
		vector<string> columns;
		vector<string> columnNames;
		const Relation* table;
	public:
		// Constructors
		Row(const Relation* tab, string primary, vector<string> cols);
		Row(const Relation* tab, Row r);
		// Gets index of data related to Column Name
		int findIndex(string colName);
		// Get data from Row
		string get(string colName);
		// Get Primary Key
		string getPK();
		// Sets column data
		template <typename T>
		bool set(string colName, T data);
		template <typename T>
		void set(int colIndex, T data);
		void setTable(Relation* t);
		void setColumnNames(vector<string> colNames);
		// Adder
		void addColumn(string name);
		void addColumns(vector<string> names);
		// For outputting to .db file
		const string operator[](int i);
		// Getters of columns and column names
		const vector<string> getColumns();
		const vector<string> getColumnNames();
};

// Function to scan first line of .db file
vector<attribute> DefineAttributes(stringstream& data);

// Parses .db for key definition
vector<string> DefineKey(stringstream& data);

// Output operator outputs relation to specified .db format
ostream& operator<<(ostream& out, Relation& table);

// Input operator loads a relation from input file
istream& operator>>(istream& in, Relation& table);

// Overloaded equality operator for rows
bool operator==(Row& lrow, Row& rrow);

// Overloaded equality operator for attribute
bool operator==(const attribute& lattr, const attribute& rattr);


class DBengine {
	private:
		vector<Relation*> tables;
		Relation* table;
		ifstream myfile;
	public:
		DBengine() {}
		~DBengine() { 
			for (int i = 0; i < tables.size(); ++i)
				delete tables[i]; 
			myfile.close();
		}
		// Opens .db file, loads into tables
		bool open(string fileName);
		// Closes myfile filestream
		bool close(string fileName);
		void exitEngine();
		bool write(string fileName);
		void show(string tableName);
		Relation* getTable(string fromName);
		bool tableExists(string tableName);
		// Sets table to relation specified by argument name
		string select(string tableName, vector<string> colNames);
		string project(string tableName, vector<string> colNames);
		string rename(string tableName, vector<string> colNames);
		void output();
		void create(string tableName, vector<attribute> attrVect, vector<string> primaryKeys, string fromName);
		void insert(string tableName, vector<string> rowData);
		bool del(string tableName, vector<string> condition_one, vector<string> condition_two, vector<string> comparisons);
		bool update(string tableName, vector<string> columnNames, vector<string> values,
						vector<string> conditionOne, vector<string> conditionTwo, vector<string> comparisons);

		vector<attribute> getTableAttributes(string name);
		vector<string> getTableKeys(string name);
		// Performs relational algebra and returns name of intermediate table
		string relationUnion(string ltable, string rtable);
		string relationCross(string ltable, string rtable);
		string relationDiff(string ltable, string rtable);
};

#endif
