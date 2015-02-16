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

bool DBengine::open(string name) {
	string fileName = name + ".db";
	myfile.open(fileName.c_str());
	if (myfile.is_open()) {
		tables.push_back(new Relation(name));
		myfile >> *tables[tables.size()-1];
	}
	return false;
}

bool DBengine::close() {
	myfile.close();
	if (!(myfile.is_open())) {
		return true;
	}
	return false;
}

void DBengine::exit() {
	std::exit(0);
}

// Gokul's function
void DBengine::write(string fileName, vector<attribute> attrVect, vector<string> primaryKey) {
	/*
	ofstream outfile;
	if (!ifstream(fileName)) { //if the file does not exist
		create(fileName, attrVect, primaryKey); //create the .db file
	}
	outfile.open(fileName);

	if (outfile.is_open()){
		for (int i = 0; i < attrVect.size(); ++i) {
			outfile << attrVect[i].attributeName << '|' << attrVect[i].attributeType << '|' << attrVect[i].attributeSize << " ";
		}
	}
	outfile.close();
	*/
}



// Gokul's function
void DBengine::show(string fileName) { //prints contents of a file to screen
	/*
	ifstream infile(fileName);
	if (infile.fail()){
		cout << "File failed to open\n";
		exit();
	}

	cout << infile.rdbuf();
	infile.close();
	*/
}

// Cody's function
void DBengine::select(string tableName) {
	for (int i = 0; i < tables.size(); ++i) {
		string name = tables[i]->getName();
		if (name == tableName)
			table = tables[i];
	}
}

void DBengine::output() {
	cout << *table << endl << endl;
}

// Quintin's function
void DBengine::create(string fileName, vector<attribute> attrVect, vector<string> primaryKey) {
	/*Open an output stream for a new .db file*/
	ofstream outfile(fileName.c_str());
	/*Writes first line of file with attribute information*/
	if (outfile.is_open()){
		for (int i = 0; i < attrVect.size(); ++i) {
			outfile << attrVect[i].attributeName << '|' << attrVect[i].attributeType << '|' << attrVect[i].attributeSize << " ";
		}
	}
	/*Saves the file*/
	outfile.close();
}

/*
Quintin's function
Input:	Table Name (string)
New Attribute (attribute)
*/
void DBengine::insert(string tableName, attribute newAttr) {
	ifstream in(tableName.c_str());
	vector<string> fileHolder;
	string line;
	if (in.is_open()){
		while (getline(in, line)) {
			fileHolder.push_back(line);
		}
	}
	in.close();
	string newColumnList = newAttr.attributeName + '|' + newAttr.attributeType + '|';
	newColumnList = newColumnList + char(newAttr.attributeSize) + " " + fileHolder[0];
	fileHolder[0] = newColumnList;
	ofstream outfile(tableName.c_str());
	if (outfile.is_open()){
		for (int i = 0; i < fileHolder.size(); ++i) {
			outfile << fileHolder[i] << endl;
		}
	}
	outfile.close();
}

// Jennings' function
// deletes specified row by content given or deletes entire table with '*' indicator
template <typename T>
void DBengine::del(Relation* table, string colName, T rowToDel) {
	vector<string> rows = table.getRows();
	vector<attribute> attributesVect = table.attributes;
	// '*' signals to delete entire table
	if (colName.compare('*') == 0 || rowToDel == '*') {
		for (int i = 0; i < table.getSize(); ++i) {
			table.deleteRow(j);
		}
	}
	else for (int j = 0; j < rows.size(); ++j) {
		if (rows[j].get(colName) == rowToDel) {
			table.deleteRow(j);
		}
	}
}

// Jennings' function
bool DBengine::update(Relation* table, int rowIndex, string colName, T whatToUpdate) {
	Row changedRow = table.getRow(rowIndex);
	if (changedRow.set(colName, whatToUpdate)) {
		return true;
	}
	else return false;
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