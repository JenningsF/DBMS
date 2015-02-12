//********************************************************
// Name			: 
// Date			: 13 Febuary 2015
// Subject		: CSCE 315-504
// Assignment	: Project 2: DBMS
// Updated		: 11 February 2015
// Description	: Database Engine where functions will be defined
//********************************************************

#include "DBengine.h"

using namespace std;

bool DBengine::open(string fileName) {
	myfile.open(fileName);
	
	if (myfile.is_open()) {
		return true;
	}
	else return false;
}

bool DBengine::close(string fileName) {
	myfile.close(fileName);

	if (!(myfile.is_open())) {
		return true;
	}
	else return false;
}

void DBengine::exit() {
	exit(0);
}

// Gokul's function
void DBengine::write() {

}

// Gokul's function
void DBengine::show(string fileName) {

}

// Cody's function
void DBengine::select() {

}

// Quintin's function
void DBengine::create(string fileName, vector<attributes> attrVect, vector<string> primaryKey) {
	/*Open an output stream for a new .db file*/
	ofstream outfile (filename);
	/*Writes first line of file with attribute information*/
	for (int i = 0; i < attrVect.size(); ++i) {
		outfile << attrVect[i].attrName << '|' << attrVect[i].type << '|' << attrVect[i].attributeSize << " ";
	}
	/*Saves the file*/
	outfile.close();
}

// Quintin's function
void DBengine::insert(string tableName) {

}

// Jennings' function
void DBengine::del() {

}

// Jennings' function
void DBengine::update() {

}
