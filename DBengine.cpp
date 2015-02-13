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
void DBengine::create(string fileName, vector<attribute> attrVect, vector<string> primaryKey) {
	/*Open an output stream for a new .db file*/
	ofstream outfile (filename);
	/*Writes first line of file with attribute information*/
	if (outfile.is_open()){
		for (int i = 0; i < attrVect.size(); ++i) {
			outfile << attrVect[i].attrName << '|' << attrVect[i].type << '|' << attrVect[i].attributeSize << " ";
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
	ifstream in(tableName);
	vector<string> fileHolder;
	string line;
	if(in.is_open()){
		while( getline(in, line){
			fileHolder.push_back(line);
		}
	}
	in.close();
	string newColumnList = newAttr.attrName + '|' + newAttr.type + '|';
	newColumnList=newColumnList + newAttr.attributeSize + " " + fileHolder[0];
	fileHolder[0] = newColumnList;
	ofstream outfile (tableName)
	if (outfile.is_open()){
		for (int i = 0; i < fileHolder.size(); ++i){
			outfile << fileHolder[i] << endl;
		}
	}
	outfile.close();
}

// Jennings' function
void DBengine::del() {

}

// Jennings' function
void DBengine::update() {

}
