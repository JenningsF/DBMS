//********************************************************
// Name			: 
// Date			: 13 Febuary 2015
// Subject		: CSCE 315-504
// Assignment	: Project 2: DBMS
// Updated		: 28 February 2015
// Description	: Database Engine where functions will be declared
//********************************************************

#ifndef _ATTRIbUTES_
#define _ATTRIbUTES_ 1
#include <string>
#include <vector>

using namespace std;
// Error classes
class ColumnNotFound{};
class RowNotFound{};
class DataNotFound{};

//move to new header file
enum string_command{eOpen, eClose, eInsert, eCreate, eWrite, eExit, eUpdate, eRename, eProject, eDelete, eShow, eSelect, eUnion, eDiff, eCross, ERROR = 100};
string_command hashit(string const& inString);

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


struct element {
	string viewName;
	string fromName;
	string_command command;
	string query_type;
	vetor<string> column; 
	vector<string> value;
	vector<string> condition_one;
	vector<string> comparisons;
	vector<string> condition_two;
	vector<string> attributes;
	vector<attribute> attribs;
};

#endif