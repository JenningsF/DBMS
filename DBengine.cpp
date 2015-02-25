//********************************************************
// Name			: Quintin Denman
//			    : Jennings Fairchild
//				: Cody Taylor
//				: Gokul Nune
// Date			: 13 February 2015
// Subject		: CSCE 315-504
// Assignment	: Project 2: DBMS
// Updated		: 13 February 2015
// Description	: Database Engine where functions will be defined
//********************************************************

#include "DBengine.h"

using namespace std;

/*
-- DONE --
Open file and read into vector of Relations and then close file
*/
bool DBengine::open(string fileName) {
	ifstream myfile;
	string fileExtension = fileName.substr(fileName.size() - 3, 3);
	if (fileExtension.compare(".db") != 0) {	// Checks is file extension exists
		fileName += ".db";			// If it doesn't, adds '.db' extension
	}

	myfile.open(fileName.c_str());
	if (myfile.is_open()) {
		tables.push_back(new Relation(fileName));
		myfile >> *tables[tables.size()-1];
		myfile.close();
		return true;
	}
	return false;
}

/*
-- DONE --
Calls write function then closes the file and deletes from the vector of Relations
*/
bool DBengine::close(string fileName) {
	string fileExtension = fileName.substr(fileName.size() - 3, 3);
	if (fileExtension.compare(".db") == 0) {	// Checks is file extension exists
		fileName.erase(filename.size() - 4, 3);			// If it does, delete '.db' extension
	}
	write(fileName);
	for (int i = 0; i < tables.size(); ++i) {
		string relationName = tables[i]->getName();
		if(relationName == tableName) {
			delete tables[i];
			vector<Relation>::iterator iter = tables.begin() + i;
			tables.erase(iter);
		}
	}
}

/*
-- DONE --
Iterates through vector of Relations and calls close on all relations
that are being used and then exits the program
*/
void DBengine::exitEngine() {
	int i = 0;
	while (tables.size() != 0) {
		close(tables[i]->getName());
	}
	exit(0);
}

/*
-- DONE --
Opens the file, writes changes made to the Relation, and then closes the file that was opened
*/
bool DBengine::write(string fileName) {
	ofstream outfile;
	string fileExtension = fileName.substr(fileName.size() - 3, 3);x
	if (fileExtension.compare(".db") != 0) {	// Checks is file extension exists
		string fileName = name + ".db";			// If it doesn't, adds '.db' extension
	}

	outfile.open(fileName.c_str(), ios_base::app);
	if (outfile.is_open()) {
		for (int i = 0; i < tables.size(); ++i) {
			string relationName = tables[i]->getName();
			if(relationName == fileName)
				outfile << tables[i] << endl;
		}
		outfile.close();
		return true;
	}
	return false;
}



/*
-- DONE --
Prints out table that is passed as an argument
*/
void DBengine::show(string tableName) {
	for (int i = 0; i < tables.size(); ++i) {
		string relationName = tables[i]->getName();
		if(relationName == tableName)
			cout << tables[i] << endl;
	}
}

/*
-- DONE --
Selects a portion or an entire Relation and creates a view with selected data
Note: Does not create file unless create/write function is called
*/
Relation* DBengine::select(string tableName, vector<string> colNames, char allTableIndicator) {
	for (int i = 0; i < tables.size(); ++i) {
		if (tables[i]->getName() == tableName) 
			Relation* tempTable = tables[i];
	}
	if (allTableIndicator == '*') {
		Relation* wholeTable = new Relation(tempTable->getName(), tempTable->attributes);
		return wholeTable;
	}
	else {
		vector<Row> rows = tempTable->getRows();
		vector<attribute> tempAttributes = tempTable->attributes;
		vector<attribute> newAttributes;
		// Iterate through both column name vectors to find matches
		for (int j = 0; j < colNames.size(); ++j) {
			for (int k = 0; k < tempAttributes.size(); ++k) {
				if (colNames[j].compare(tempAttributes[k]->attributeName) == 0) {
					newAttributes.push_back(tempAttributes[k]);
				}
			}
		}
		Relation* newRel = new Relation(tableName, newAttributes);
		newRel.setColumnNames();
		newRel.setRows(rows);
		vector<Row> newRows = newRel->getRows();
		vector<string> newColumns = newRel.getColumnNames();
		// Sets row data
		for (int m = 0; m < newRel.getColumnSize(); ++m) {
			for (int n = 0; n < newRel.getSize(); ++n) {
				newRows[l].set(newColumns[m], rows[n].get(newColumns[m]));
			}
		}
		return newRel;
	}
}

/*
-- Strictly testing --
*/
void DBengine::output() {
	cout << *table << endl << endl;
}

/*
-- DONE --
Initilizes a Relation object, then pushes back on tables vector
*/
void DBengine::create(string tableName, vector<attribute> attrVect, vector<string> primaryKeys) {
	Relation* rel = new Relation(tableName, attrVect);
	rel.setKeyParameters(primaryKeys);
	rel.setColumnNames();
	tables.push_back(rel);
}

/*
-- DONE --
Insert a Row into a Relation
*/
void DBengine::insert(string tableName, vector<attribute> rowData) {
	for (int i = 0; i < tables.size(); ++i) {
		if (tables[i]->getName() == tableName) {
			Relation* tempTable = tables[i]
			break;
		}
	}

	vector<string> keys = tempTable->keyParameters
	std::vector<string> ;
	string primaryKey = "";
	// Sets primary key for new row
	for (int j = 0; j < keys.size(); ++j) {
		for (int k = 0; k < rowData.size(); ++k) {
			if (keys[j].compare(rowData[k].attributeName) == 0) {
				primaryKey += rowData[k]
			}
		}
	}
	// Check if row data matches the type and name of the column they are inserted into
	for (int j = 0; j < rowData.size(); ++j) {
		if (!(rowData[j].attributeType.compare(attributes[j].attributeType) == 0 && 
				rowData[j].attributeName.size() <= attributes[j].attributeSize)) {
			// This will be an error
			return;
		}
		else tempTable->addRow(primaryKey);
	}
}

/*
-- DONE --
Delete either a Row(s), Column(s), or entire Relationfile
*/
template <typename T>
void DBengine::del(string tableName, string colName, T rowToDel) {
	for (int i = 0; i < tables.size(); ++i) {
		if (tables[i]->getName() == tableName) {
			Relation* tempTable = tables[i]
			break;
		}
	}	
	vector<Row> rows = tempTable->getRows();
	// '*' signals to delete entire table
	if (colName.compare('*') == 0 || rowToDel == '*') {
		for (int i = 0; i < tempTable->getSize(); ++i) {
			tempTable->deleteRow(i);
		}
	}
	// Delete row matching colName and the info given.
	else for (int j = 0; j < rows.size(); ++j) {
		if (rows[j].get(colName) == rowToDel) {
			tempTable->deleteRow(j);
		}
	}
}

/*
-- DONE --
Changes specified data whether it be a certain cell, row, or column
*/
bool DBengine::update(string tableName, int rowIndex, string colName, T whatToUpdate) {
	for (int i = 0; i < tables.size(); ++i) {
		if (tables[i]->getName() == tableName) {
			Relation* tempTable = tables[i]
			break;
		}
	}
	Row changedRow = table.getRow(rowIndex);
	if (changedRow.set(colName, whatToUpdate)) {
		return true;
	}
}

// Cody's functions
// Output operator outputs relation to specified .db format
ostream& operator<<(ostream& out, Relation& table) {
	// List attribute name type (size if string) 
	for (int i = 0; i < table.attributes.size(); ++i) {
		out << table.attributes[i].attributeName << " ";
		if (table.attributes[i].attributeName != "key")
			out << table.attributes[i].attributeType << " ";
		if (table.attributes[i].attributeType == "string")
			out << table.attributes[i].attributeSize << " ";
		if (table.attributes[i].attributeName == "key") {
			for (int j = 0; j < table.keyParameters.size(); ++j) {
				out << table.keyParameters[j] << " ";
			}
		}
		if (table.attributes.size()-1 != i)
			out << ", ";
	}
	out << "\n";
	for (int k = 0; k < table.getSize(); ++k) {
		for (int l = 0; l < table.getColumnSize(); ++l) {
			out << table.getRow(k)[l];
			if (l != table.getColumnSize() - 1)
				out << " /,/ ";
		}
		out << "\n";
	}
	return out;
}


// Overloaded equality operator for rows
bool operator==(const Row& lrow, const Row& rrow) {
	if (lrow.getPK() == rrow.getPK() && lrow.getColmns() == rrow.getColmns()
		&& lrow.getColumnNames() == rrow.getColumnNames())
		return true;
	else return false;
}


// Overloaded equality operator for attribute
bool operator==(const attribute& lattr, const attribute& rattr) {
	if (lattr.attributeName == rattr.attributeName 
		&& lattr.attributeType == rattr.attributeType
		&& lattr.attributeSize == rattr.attributeSize
		&& lattr.isPk == rattr.isPk)
		return true;
	else return false;
}

// Overloaded set union operator for relation
Relation* operator+(Relation& ltable, Relation& rtable) {
	if (ltable.keyParameters == rtable.keyParameters 
		&& ltable.attributes == rtable.attributes) 
	{
		Relation* table = new Relation("", ltable.attributes);
		table->addRows(ltable.getRows());
		table->addRows(rtable.getRows());
		return table;
	}
	else return NULL;
}

// Overloaded set difference operator for relation
Relation* operator-(Relation& ltable, Relation& rtable) {	
	if (ltable.keyParameters == rtable.keyParameters 
		&& ltable.attributes == rtable.attributes) 
	{
		Relation* table = new Relation("", ltable.attributes);
		table->addRows(ltable.getRows());
		for (int i = 0; i < rtable.getSize(); ++i) {
			for (int j = 0; j < table.getSize(); ++j) {
				if (rtable.getRow(i).getPK() == table->getRow(j).getPK()) {
					table->deleteRow(j);
					break;
				}
				else continue;
			}
		}
		return table;
	}
	else return NULL;
}

// Relation Function Definitions
Row Relation::getRow(int i) { 
	if (i < size)
		return rows[i];
	else throw RowNotFound();
}

// Add row to table
void Relation::addRow(string primary) {
	rows.push_back(Row(this, primary, columnNames));
	++size;
}

// Add column to table
void Relation::addAttribute(attribute attrib) {
	attributes.push_back(attrib);
	columnNames.push_back(attrib.attributeName);
	++colSize;
	for (int i = 0; i < rows.size(); ++i) {
		rows[i].columnNames.push_back(attrib.attributeName);
		rows[i].columns.push_back("");
	}
}

// Input operator loads a relation from input file
istream& operator>>(istream& in, Relation& table) {
	string line = "";
	string delimiter = " /,/ ";
	size_t pos = 0;
	vector<Row> rows;
	vector<string> tempData;
	getline(in, line, '\n');
	stringstream attribData(line);
	vector<attribute> attribs = DefineAttributes(attribData);
	table.setAttributes(attribs);
	table.setKeyParameters(DefineKey(attribData));
	table.setColumnNames();
	while (getline(in, line, '\n')) {
		while ((pos = line.find(delimiter)) != string::npos) {
			tempData.push_back(line.substr(0, pos));
			line.erase(0, pos + delimiter.length());
		}
		if (line.size() != 0) tempData.push_back(line);
		rows.push_back(Row(&table, tempData[tempData.size() - 1], table.getColumnNames()));
		table.incrementSize();
		for (int i = 0; i < tempData.size(); ++i) {
			if (attribs[i].attributeType == "string" || attribs[i].attributeType == "key")
				rows[rows.size() - 1].set(attribs[i].attributeName, tempData[i]);
			else if (attribs[i].attributeType == "int")
				rows[rows.size() - 1].set(attribs[i].attributeName, atoi(tempData[i].c_str()));
			else if (attribs[i].attributeType == "double")
				rows[rows.size() - 1].set(attribs[i].attributeName, atof(tempData[i].c_str()));
		}
		tempData.clear();
	}
	table.setRows(rows);
	return in;
}

// Function to scan first line of .db file
vector<attribute> DefineAttributes(stringstream& data) {
	int size = 0;
	string name = "";
	string type = "";
	string delim = "";
	vector<string> keys;
	vector<attribute> attribs;
	while (data >> name) {
		if (name == "key") {
			attribs.push_back(attribute("key", "key", true));
			return attribs;
		}
		else
			data >> type;
		if (type == "string")
			data >> size;

		data >> delim;
		attribs.push_back(attribute(name, type, false, size));
	}
	return attribs;
}

// Parses .db for key definition
vector<string> DefineKey(stringstream& data) {
	string parameter = "";
	vector<string> keys;
	while (data >> parameter) {
		keys.push_back(parameter);
	}
	return keys;
}