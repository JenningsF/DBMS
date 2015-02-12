//********************************************************
// Name			: 
// Date			: 13 Febuary 2015
// Subject		: CSCE 315-504
// Assignment	: Project 2: DBMS
// Updated		: 11 February 2015
// Description	: Database Engine where functions will be declared
//********************************************************

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct attribute {
	string attributeName;
	string type;
	int attributeSize;
	bool isPk;
};

bool open();
bool close();
void exit();
void write();
void show();
void select();
void create(string filename);
void insert();
void del();
void update();
