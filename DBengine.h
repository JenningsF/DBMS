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
	bool isPk;
	attribute() : attributeName(""), attributeType(""),
		attributeSize(0), isPk(false) {}
	attribute(string name, string type, bool pk, int size = 0) {
		attributeName = name;
		attributeType = type;
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
		Relation() : name("") {}
		Relation(string n, vector<attribute> attribs = vector<attribute>()) {
			 name = n;
			 attributes = attribs;
			 size = 0;
		}
		// Getter functions
		string getName() {return name;}
		int getSize() { return size; }
		int getColumnSize() { return colSize; }
		vector<string> getColumnNames() { return columnNames; }
		const vector<Row> getRows() { return rows; }
		Row getRow(int i);
		// Setter functions
		void setName(string n) {name = n;}
		void setAttributes(vector<attribute> attribs) { 
			attributes = attribs; 
			colSize = attribs.size();
		}
		void setKeyParameters(vector<string> keys) { keyParameters = keys; }
		void setRows(vector<Row> r) {
			rows = r;
			size = rows.size();
		}
		void setColumnNames() {
			for (int i = 0; i < attributes.size(); ++i) {
				columnNames.push_back(attributes[i].attributeName);
			}
		}
		// Add row to table
		void addRow(string primary);
		void addRow(Row r) {
			bool exists = false;
			for (int j = 0; j < rows.size(); ++j) {
				if (rows[j].getPK() == r.getPK())
					exists = true;
				else continue;
			}
			if (!exists) {
				rows.push_back(Row(this, r));
				++size;
			}			
		}
		void addRows(vector<Row> r) {
			for (int i = 0; i < r.size(); ++i) {
				addRow(r[i]);
			}
		}
		void incrementSize() { ++size; }
		// Add column to table
		void addAttribute(attribute attrib);
		// Remove row based on index
		void deleteRow(int i) {
			if (i < size) {
				rows.erase(rows.begin() + i);
				--size;
			}
			else throw RowNotFound();
		}
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

	// Constructor
	Row(const Relation* tab, string primary = "",
		vector<string> cols = vector<string>())
	{
		table = tab;
		PK = primary;
		columnNames = cols;
		columns = vector<string>(cols.size());
	}

	Row(const Relation* tab, Row r) {
		table = tab;
		PK = r.PK;
		columnNames = r.columnNames;
		columns = r.columns;
	}

	// Gets index of data related to Column Name
	int findIndex(string colName) {
		for (int i = 0; i < columnNames.size(); ++i) {
			if (columnNames[i] != colName)
				continue;
			return i;
		}
		return -1; // Not found
	}

	// Get data from Row
	template <typename T>
	T get(string colName) {
		int index = findIndex(colName);
		if (columns.size() > index && index != -1) {
			if (table->attributes[index].attributeType == "string")
				return columns[index];
			else if (table->attributes[index].attributeType == "int")
				return atoi(columns[index].c_str());
			else if (table->attributes[index].attributeType == "double")
				return atof(columns[index].c_str());
		}
		else throw ColumnNotFound();
	}

	// Get Primary Key
	string getPK() { return PK; }

	// Sets column data
	template <typename T>
	bool set(string colName, T data) {
		int index = findIndex(colName);
		// Ensures columnName and columns vectors are same size
		if (columns.size() > index && index != -1) {
			stringstream buffer;
			// Make data a string for storing
			buffer << data;
			// Ensure it complies with data size spec.
			if (table->attributes[index].attributeType == "string" &&
				table->attributes[index].attributeSize < buffer.str().size())
				return false;

			columns[index] = buffer.str();
		}
		else return false;

		return true;
	}

	void setTable(Relation* t) {
		table = t;
	}

	// For outputting to .db file
	const string operator[](int i) { 
		if (i >= 0 && i < columns.size()) 
			return columns[i]; 
		else throw DataNotFound();
	}

	// Getters of columns and column names
	const vector<string> getColumns() { return columns; }
	const vector<string> getColumnNames() { return columnNames; }
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
bool operator==(const Row& lrow, const Row& rrow);

// Overloaded equality operator for attribute
bool operator==(const attribute& lattr, const attribute& rattr);

// Overloaded set union operator for relation
Relation operator+(Relation& ltable, Relation& rtable);

// Overloaded set difference operator for relation
Relation operator-(Relation& ltable, Relation& rtable);

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
		bool open(string name);
		// Closes myfile filestream
		bool close();
		void exit();
		void write(string tableName);
		void show(string filename);
		// Sets table to relation specified by argument name
		void select(string tableName);
		void output();
		void create(string fileName, vector<attribute> attrVect, vector<string> primaryKey);
		void insert(string tableName, attribute newAttr);
		template <typename T>
		void del(Relation* table, string colName, T rowToDel);
		template <typename T>
		void update(Relation* table, int rowIndex, string colName, T whatToUpdate);
};

#endif