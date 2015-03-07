#include "libtest/DbmsApi.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
	DBMSParse("CREATE TABLE animals (name VARCHAR(20), kind VARCHAR(8), years INTEGER) PRIMARY KEY (name, kind);");
	Table tab = getRelation("animals");
	vector<string> first = {"Joe", "dog", "41"};
	tab.addTuple(first);
	cout << tab.getTuple(0)[0] << tab.getTuple(0)[1] << endl;

	return 0;
}