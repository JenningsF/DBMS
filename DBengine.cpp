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
/*********************************************************
					Relation Functions
*********************************************************/

// Constructors
Relation::Relation() : name("") {}
Relation::Relation(string n, vector<attribute> attribs = vector<attribute>()) {
	 name = n;
	 attributes = attribs;
	 colSize = attribs.size();
	 size = 0;
}

Relation::Relation(string tableName, Relation* tab)  {
	name = tableName;
	attributes = tab->attributes;
	keyParameters = tab->keyParameters;
	addRows(tab->getRows());
	size = tab->getRows().size();
	colSize = tab->colSize;
	columnNames = tab->columnNames;
}

// Getter functions
string Relation::getName() { return name; }
int Relation::getSize() { return size; }
int Relation::getColumnSize() { return colSize; }
vector<string> Relation::getColumnNames() { return columnNames; }
vector<Row> Relation::getRows() { return rows; }
Row& Relation::getRow(int i) { 
	if (i < size && i > -1)
		return rows[i];
	else throw RowNotFound();
}

// Setter functions
void Relation::setName(string n) { name = n; }
void Relation::setAttributes(vector<attribute> attribs) { 
	attributes = attribs; 
	colSize = attribs.size();
}
void Relation::setKeyParameters(vector<string> keys) { keyParameters = keys; }
void Relation::setRows(vector<Row> r) {
	rows = r;
	size = rows.size();
}
void Relation::setColumnNames() {
	for (int i = 0; i < attributes.size(); ++i) {
		columnNames.push_back(attributes[i].attributeName);
	}
	colSize = columnNames.size();
}
// Add row to table
void Relation::addRow(string primary) {
	rows.push_back(Row(this, primary, columnNames));
	++size;
}
void Relation::addRow(Row r) {
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
void Relation::addRows(vector<Row> r) {
	for (int i = 0; i < r.size(); ++i) {
		addRow(r[i]);
	}
}
void Relation::incrementSize() { ++size; }
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
// Remove row based on index
void Relation::deleteRow(int i) {
	if (i < size) {
		rows.erase(rows.begin() + i);
		--size;
	}
	else throw RowNotFound();
}

/*********************************************************
					Row Functions
*********************************************************/

// Constructors
Row::Row(const Relation* tab, string primary = "",
	vector<string> cols = vector<string>()) {
	table = tab;
	PK = primary;
	columnNames = cols;
	columns = vector<string>(cols.size());
}

Row::Row(const Relation* tab, Row r) {
	table = tab;
	PK = r.PK;
	columnNames = r.columnNames;
	columns = r.columns;
}

// Gets index of data related to Column Name
int Row::findIndex(string colName) {
	for (int i = 0; i < columnNames.size(); ++i) {
		if (columnNames[i] != colName)
			continue;
		return i;
	}
	return -1; // Not found
}

// Get data from Row
string Row::get(string colName) {
	int index = findIndex(colName);
	if (columns.size() > index && index != -1) {
		return columns[index];
	}
	else throw ColumnNotFound();
}

// Get Primary Key
string Row::getPK() { return PK; }

// Sets column data
template <typename T>
bool Row::set(string colName, T data) {
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


template <typename T>
void Row::set(int colIndex, T data) {
	if (columns.size() > colIndex && colIndex >= 0) {
		stringstream buffer;
		buffer << data;
		if (table->attributes[colIndex].attributeType == "string" &&
			table->attributes[colIndex].attributeSize < buffer.str().size())
			return;

		columns[colIndex] = buffer.str();
	}

}

// Add Column
void Row::addColumn(string name) {
	columnNames.push_back(name);
	columns.push_back("");
}

void Row::addColumns(vector<string> names) {
	for (int i = 0; i < names.size(); ++i) {
		columnNames.push_back(names[i]);
		columns.push_back("");
	}
}

// Sets the row's parent table
void Row::setTable(Relation* t) {
	table = t;
}

// For outputting to .db file
const string Row::operator[](int i) { 
	if (i >= 0 && i < columns.size()) 
		return columns[i]; 
	else throw DataNotFound();
}

// Getters of columns and column names
const vector<string> Row::getColumns() { return columns; }
const vector<string> Row::getColumnNames() { return columnNames; }

/*********************************************************
					DBengine Functions
*********************************************************/

/*
Open file and read into vector of Relations and then close file
*/
bool DBengine::open(string fileName) {
	ifstream myfile;
	string fileExtension = fileName.substr(fileName.size() - 3, 3);
	string actualName = fileName.substr(0,fileName.find('.'));
	if (fileExtension.compare(".db") != 0) {	// Checks is file extension exists
		fileName += ".db";						// If it doesn't, adds '.db' extension
	}

	myfile.open(fileName.c_str());
	if (myfile.is_open()) {
		tables.push_back(new Relation(actualName));
		myfile >> *tables[tables.size()-1];
		myfile.close();
		return true;
	}
	return false;
}

/*
Calls write function then closes the file and deletes from the vector of Relations
*/
bool DBengine::close(string fileName) {
	string fileExtension = fileName.substr(fileName.size() - 3, 3);
	if (fileExtension.compare(".db") == 0) {		// Checks is file extension exists
		fileName.erase(fileName.size() - 4, 3);		// If it does, delete '.db' extension
	}
	write(fileName);
	for (int i = 0; i < tables.size(); ++i) {
		string relationName = tables[i]->getName();
		if(relationName == fileName) {
			delete tables[i];
			vector<Relation*>::iterator iter = tables.begin() + i;
			tables.erase(iter);
		}
	}
	return true;
}

/*
Iterates through vector of Relations and calls close on all relations
that are being used and then exits the program
*/
void DBengine::exitEngine() {
	int i = 0;
	for (int i = 0; i < tables.size(); ++i) {
		delete tables[i];
	}
	exit(0);
}

/*
Opens the file, writes changes made to the Relation, and then closes the file that was opened
*/
bool DBengine::write(string fileName) {
	ofstream outfile;
	string fileExtension = fileName.substr(fileName.size() - 3, 3);
	string actualName = fileName.substr(0,fileName.find('.'));
	if (fileExtension.compare(".db") != 0) {	// Checks is file extension exists
		fileName = fileName + ".db";			// If it doesn't, adds '.db' extension
	}

	outfile.open(fileName.c_str());
	if (outfile.is_open()) {
		for (int i = 0; i < tables.size(); ++i) {
			string relationName = tables[i]->getName();
			if(relationName == actualName)
				outfile << *tables[i] << endl;
		}
		outfile.close();
		return true;
	}
	return false;
}


bool DBengine::tableExists(string tableName) {
	for (int i = 0; i < tables.size(); ++i) {
		if (tables[i]->getName() == tableName) return true;
	}
	return false;
}


/*
Prints out table that is passed as an argument
*/
void DBengine::show(string tableName) {
	for (int i = 0; i < tables.size(); ++i) {
		string relationName = tables[i]->getName();
		if(relationName == tableName)
			cout << *tables[i] << endl;
	}
}

Relation* DBengine::getTable(string tableName) {
	for (int i = 0; i < tables.size(); ++i) {
		if (tables[i]->getName() == tableName) return tables[i];
	}
}

vector<string> formatConditions(vector<string> cols) {
	vector<string> formatted;
	for (int i = 0; i < cols.size(); ++i) {
		while (cols[i].length() > 0) {
			size_t space = cols[i].find(' ');
			size_t quote = cols[i].find('"');
			size_t minimum = min(space, quote);
			formatted.push_back(cols[i].substr(0, minimum));
			cols[i].erase(0, minimum + 1);
			if (cols[i][0] == '"') cols[i].erase(0, 1);
		}

	}
	return formatted;
}

/*
Selects a portion or an entire Relation and creates a view with selected data
Note: Does not create file unless create/write function is called
*/
string DBengine::select(string tableName, vector<string> colNames) {
	srand(time(NULL));
	int raNum = 0;
	string newName = tableName + "Select";
	while (tableExists(newName)) {
		raNum = rand() % 10;
		newName += raNum;
	}
	Relation* tempTable = new Relation(newName, getTable(tableName));
	vector<Row> rows = getTable(tableName)->getRows();
	vector<attribute> tempAttributes = getTable(tableName)->attributes;
	vector<attribute> newAttributes;
	
	vector<string> formattedConditions = formatConditions(colNames);
	// Iterate through both column name vectors to find matches
	for (int j = 0; j < formattedConditions.size(); ++j) {
		if (j + 2 >= formattedConditions.size()) break;
		string left = formattedConditions[j];
		++j;
		string oper = formattedConditions[j];
		++j;
		string right = formattedConditions[j];
		for (int k = rows.size() - 1; k >= 0; --k) {
			if (oper == "==") {
				if (rows[k].get(left) != right) rows.erase(rows.begin() + k);
			}
			else if (oper == "!=") {
				if (rows[k].get(left) == right) rows.erase(rows.begin() + k);
			}
		}
		if (j + 1 < formattedConditions.size()) { 
			if (formattedConditions[j + 1] == "&&") formattedConditions.erase(formattedConditions.begin() + j + 1);
		}
	}
	tempTable->setRows(rows);
	tables.push_back(tempTable);
	return tempTable->getName();
}

// Projects specified column names from the specified table to a new table.
string DBengine::project(string tableName, vector<string> colNames) {
	srand(time(NULL));
	int raNum = 0;
	string newName = tableName + "Project";
	while (tableExists(newName)) {
		raNum = rand() % 10;
		newName += raNum;
	}
	bool contains = false;
	for (int i = 0; i < colNames.size(); ++i) {
		if (colNames[i] == "key") contains = true;
	}
	if (!contains) colNames.push_back("key");
	vector<attribute> temp = getTable(tableName)->attributes;
	vector<attribute> newAttribs;
	for (int i = 0; i < temp.size(); ++i) {
		for (int j = 0; j < colNames.size(); ++j) {
			if (colNames[j] == temp[i].attributeName) newAttribs.push_back(temp[i]);
		}
	}
	Relation* tempTable = new Relation(newName, newAttribs);
	vector<Row> rows = getTable(tableName)->getRows();
	vector<Row> newRows;
	for (int i = 0; i < rows.size(); ++i) {
		newRows.push_back(Row(tempTable, rows[i].getPK(), colNames));
		for (int j = 0; j < colNames.size(); ++j) {
			string data = rows[i].get(colNames[j]);
			newRows[newRows.size() - 1].set(colNames[j], data);
		}
	}
	tempTable->setRows(newRows);
	tables.push_back(tempTable);
	return tempTable->getName();
}


/*
-- Strictly testing --
*/
void DBengine::output() {
	cout << *table << endl << endl;
}

/*
Initilizes a Relation object, then pushes back on tables vector
*/
void DBengine::create(string tableName, vector<attribute> attrVect, vector<string> primaryKeys, string fromName = "") {
	if (fromName == "") {
		Relation* rel = new Relation(tableName, attrVect);
		rel->setKeyParameters(primaryKeys);
		rel->setColumnNames();
		tables.push_back(rel);
	}
	else {
		Relation* rel = new Relation(tableName, getTable(fromName));
		tables.push_back(rel);
	}
	
}

/*
Insert a Row into a Relation
*/
void DBengine::insert(string tableName, vector<string> rows) {
	Relation* tempTable = NULL;
	for (int i = 0; i < tables.size(); ++i) {
		if (tables[i]->getName() == tableName) {
			tempTable = tables[i];
			break;
		}
	}

	vector<string> keys = tempTable->keyParameters;
	vector<attribute> attributes = tempTable->attributes;
	vector<attribute> rowData = tempTable->attributes;
	for (int i = 0; i < rows.size(); ++i) {
		rowData[i].entryData = rows[i];
	}
	string primaryKey = "";
	// Sets primary key for new row
	for (int j = 0; j < keys.size(); ++j) {
		for (int k = 0; k < rowData.size(); ++k) {
			if (keys[j].compare(rowData[k].attributeName) == 0) {
				primaryKey += rowData[k].entryData;
			}
		}
	}
	/* Check if row data matches the type and name of the column they are inserted into
	for (int j = 0; j < rowData.size(); ++j) {
		if (!(rowData[j].attributeType.compare(attributes[j].attributeType) == 0 && 
				rowData[j].attributeName.size() <= attributes[j].attributeSize)) {
			// This will be an error
		}
		else continue;
	}*/

	tempTable->addRow(Row(tempTable, primaryKey, tempTable->getColumnNames()));
	for (int i = 0; i < rowData.size(); ++i) {
		tempTable->getRow(tempTable->getSize() - 1).set(rowData[i].attributeName, rowData[i].entryData);
	}
	tempTable->getRow(tempTable->getSize() - 1).set("key", primaryKey);
}


/*
Delete either a Row(s), Column(s), or entire Relationfile
*/
bool DBengine::del(string tableName, vector<string> condition_one, vector<string> condition_two, vector<string> comparisons) {
	Relation* tempTable = NULL;
	for (int i = 0; i < tables.size(); ++i) {
		if (tables[i]->getName() == tableName) {
			tempTable = tables[i];
			break;
		}
	}	
	vector<int> indeces;
	vector<Row> rows = tempTable->getRows();
	for (int i = 0; i < rows.size(); ++i){
		indeces.push_back(i);
	}
	for (int i = 0; i < comparisons.size() + 1; ++i) {
		if(i > 0) {
			if (comparisons[i-1] != "&&") {
				cout << "Error: Invalid comparison\n";
				return false;
			}
		}
		for (int j = 0; j < indeces.size(); ++j) {
			if (i >= condition_one.size()) { break; }
			if(rows[indeces[j]].get(condition_one[i]) != condition_two[i]) {
				indeces.erase(indeces.begin() + j);
				--j;
			}
		}
	}
	int count = 0;
	for (int i = 0; i < indeces.size(); ++i) {
		tempTable->deleteRow(indeces[i]-count);
		++count;
	}
}

/*
Changes specified data whether it be a certain cell, row, or column
*/
bool DBengine::update(string tableName, vector<string> columnNames, vector<string> values,
						vector<string> conditionOne, vector<string> conditionTwo, vector<string> comparisons) {
	Relation* tempTable;
	for (int i = 0; i < tables.size(); ++i) {
		if (tables[i]->getName() == tableName) {
			tempTable = tables[i];
			break;
		}
	}
	// Checks if condition and comparisons contain complete bool statements
	if (!(conditionOne.size() == conditionTwo.size() && comparisons.size() == conditionOne.size() - 1)) {
		return false;
	}

	vector<Row> tempRows = tempTable->getRows();
	vector<int> goodRowIndexes;	// Keeps indexes of each row that meets criteria
	for (int j = 0; j < tempRows.size(); ++j) {
		goodRowIndexes.push_back(j);
	}

	for (int k = 0; k < comparisons.size() + 1; ++k) {
		for (int l = 0; l < tempRows.size(); ++l) {
			if (!(tempRows[l].get(conditionOne[k]) == conditionTwo[k])) {
				tempRows.erase(tempRows.begin() + l);
				goodRowIndexes.erase(goodRowIndexes.begin() + l);
			}
		}
		// Excludes rows that do not meet all criteria if there's more than one
		if (k > 0 && comparisons[k - 1] == "&&") {
			for (int m = 0; m < tempRows.size(); ++m) {
				if (!(tempRows[m].get(conditionOne[k - 1]) == conditionTwo[k - 1] && tempRows[m].get(conditionOne[k]) == conditionTwo[k])) {
					tempRows.erase(tempRows.begin() + m);
					goodRowIndexes.erase(goodRowIndexes.begin() + m);
				}
			}
		}
	}

	// Checks if columnNames and values are paired
	if (!(columnNames.size() == values.size())) {
		return false;
	}

	for (int n = 0; n < columnNames.size(); ++n) {
		for (int q = 0; q < goodRowIndexes.size(); ++q) {
			tempTable->getRow(q).set(columnNames[n], values[n]);
		}
	}

	return true;
}


string DBengine::relationCross(string ltable, string rtable){
	Relation* ltab = new Relation();
	Relation* rtab = new Relation();
	for (int i = 0; i < tables.size(); ++i) {
		if (tables[i]->getName() == ltable) ltab = tables[i];
		else if (tables[i]->getName() == rtable) rtab = tables[i];
	}
	Relation* temp = (*ltab)*(*rtab);
	temp->setName(ltable + "*" + rtable);
	tables.push_back(temp);
	return temp->getName();
}


vector<attribute> DBengine::getTableAttributes(string name) {
	for (int i = 0; i < tables.size(); ++i) {
		if (tables[i]->getName() == name) return tables[i]->attributes;
	}
}

vector<string> DBengine::getTableKeys(string name) {
	for (int i = 0; i < tables.size(); ++i) {
		if (tables[i]->getName() == name) return tables[i]->keyParameters;
	}
}

string DBengine::relationUnion(string ltable, string rtable) {
	Relation* ltab = new Relation();
	Relation* rtab = new Relation();
	for (int i = 0; i < tables.size(); ++i) {
		if (tables[i]->getName() == ltable) ltab = tables[i];
		else if (tables[i]->getName() == rtable) rtab = tables[i];
	}
	Relation* temp = (*ltab)+(*rtab);
	temp->setName(ltable + "+" + rtable);
	tables.push_back(temp);
	return temp->getName();
}

string DBengine::relationDiff(string ltable, string rtable) {
	Relation* ltab = new Relation();
	Relation* rtab = new Relation();
	for (int i = 0; i < tables.size(); ++i) {
		if (tables[i]->getName() == ltable) ltab = tables[i];
		else if (tables[i]->getName() == rtable) rtab = tables[i];
	}
	Relation* temp = (*ltab)-(*rtab);
	temp->setName(ltable + "-" + rtable);
	tables.push_back(temp);
	return temp->getName();
}

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
bool operator==(Row& lrow, Row& rrow) {
	if (lrow.getPK() == rrow.getPK() && lrow.getColumns() == rrow.getColumns()
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
Relation* Relation::operator+(Relation& rtable) {
	if (keyParameters == rtable.keyParameters 
		&& attributes == rtable.attributes) 
	{
		Relation* table = new Relation("", attributes);
		table->addRows(getRows());
		table->addRows(rtable.getRows());
		table->setKeyParameters(keyParameters);
		table->columnNames = columnNames;
		table->colSize = columnNames.size();
		table->size = table->getRows().size();
		return table;
	}
	else return NULL;
}

// Overloaded set difference operator for relation
Relation* Relation::operator-(Relation& rtable) {	
	if (keyParameters == rtable.keyParameters 
		&& attributes == rtable.attributes) 
	{
		Relation* table = new Relation("", attributes);
		table->addRows(getRows());
		for (int i = 0; i < rtable.getSize(); ++i) {
			for (int j = 0; j < table->getSize(); ++j) {
				if (rtable.getRow(i).getPK() == table->getRow(j).getPK()) {
					table->deleteRow(j);
					break;
				}
				else continue;
			}
		}
		table->setKeyParameters(keyParameters);
		table->columnNames = columnNames;
		table->colSize = columnNames.size();
		return table;
	}
	else return NULL;
}

/* Overloaded cross product operator for relation */
Relation* Relation::operator*(Relation& rtable) {
	vector<attribute> attribs = this->attributes;
	for (int i = 0; i < rtable.attributes.size(); ++i) {
		if (rtable.attributes[i].attributeName != "key") {
			attribute temp_attrib = rtable.attributes[i];
			temp_attrib.isPk = false;
			attribs.insert(attribs.end() - 1, temp_attrib);
		}
	}
	Relation* table = new Relation("", attribs);
	for (int j = 0; j < this->getSize(); ++j) {
		for (int k = 0; k < rtable.getSize(); ++k) {
			Row temp_row(table, this->getRow(j));
			vector<string> colnames = rtable.getRow(k).getColumnNames();
			if (colnames[colnames.size() - 1] == "key") colnames.erase(colnames.end()-1);
			temp_row.addColumns(colnames);
			for (int l = 0; l < rtable.getRow(k).getColumns().size() - 1; ++l) {
				temp_row.set(getColumnSize() + l, rtable.getRow(k).getColumns()[l]);
			}
			table->addRow(temp_row);
		}
	}
	table->keyParameters = keyParameters;
	table->colSize = table->getRow(0).columns.size();
	return table;
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
		if (tempData.size() == 0) break;
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
