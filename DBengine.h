//********************************************************
// Name			: 
// Date			: 13 Febuary 2015
// Subject		: CSCE 315-504
// Assignment	: Project 2: DBMS
// Updated		: 11 February 2015
// Description	: Database Engine where functions will be declared
//********************************************************

#ifndef _DBEngine_
#define _DBEngine_1
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <vector>

using namespace std;

// Error classes
class ColumnNotFound{};
class RowNotFound{};
class DataNotFound{};

// Attribute struct to hold column properties
struct attribute {
	string attributeName;
	string attributeType;
	int attributeSize;
	string entryData;
	bool isPk;
	attribute() : attributeName(""), attributeType(""),
		attributeSize(0), isPk(false) {}
	attribute(string name, string type, bool pk, int size = 0) {
		attributeName = name;
		attributeType = type;
		isPk = pk;
		attributeSize = size;
	};
	// Constructor for use of inserting Row
	attribute(string name, string type, string data, bool pk, int size = 0) {
		attributeName = name;
		attributeType = type;
		entryData = data;
		isPk = pk;
		attributeSize = size;
	};
/*	attribute(string name, string type) : attributeSize(0), isPk(false) {
		attributeName = name;
		attributeType = type;
	}*/
	attribute(const attribute& a) {
		attributeName = a.attributeName;
		attributeType = a.attributeType;
		attributeSize = a.attributeSize;
		isPk = a.isPk;
	}
};

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
		// Getter functions
		string getName();
		int getSize();
		int getColumnSize();
		vector<string> getColumnNames();
		const vector<Row> getRows();
		Row getRow(int i);
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
		template <typename T>
		T get(string colName);
		// Get Primary Key
		string getPK();
		// Sets column data
		template <typename T>
		bool set(string colName, T data);
		void setTable(Relation* t);
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

// Overloaded set union operator for relation
Relation* operator+(Relation& ltable, Relation& rtable);

// Overloaded set difference operator for relation
Relation* operator-(Relation& ltable, Relation& rtable);

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
		// Sets table to relation specified by argument name
		template <typename T>
		Relation* select(string tableName, vector<string> colNames, char allTableIndicator);
		void output();
		void create(string tableName, vector<attribute> attrVect, vector<string> primaryKeys);
		void insert(string tableName, vector<attribute> rowData);
		template <typename T>
		void del(string tableName, string colName, T rowToDel);
		template <typename T>
		bool update(string tableName, int rowIndex, string colName, T whatToUpdate);
};

#endif