//********************************************************
// Name			: Quintin Denman
//			    : Jennings Fairchild
//				: Cody Taylor
//				: Gokul Nune
// Date			: 13 February 2015
// Subject		: CSCE 315-504
// Assignment	: Project 2: DBMS
// Updated		: 13 February 2015
// Description	: Database Engine where functions will be declared
//********************************************************

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct attribute {
	string attributeName;
	string type;
	int attributeSize;
	bool isPk;
};

bool open(string fileName);
bool close(string fileName);
void exit();
void write();
void show();
void select();
void create(string fileName, vector<attributes> attrVect, vector<string> primaryKey);
void insert(string tableName, attribute newAttr);
void del();
void update();
