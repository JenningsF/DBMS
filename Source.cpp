#include "DBengine.h"

 int main(){
	DBengine DB;
	DB.open("animals");
	DB.close();
	DB.open("humans");
	DB.close();
	DB.select("animals");
	DB.output();
	DB.select("humans");
	DB.output();
	return 0;
}