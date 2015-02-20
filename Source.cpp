#include "DBengine.h"

 int main(){
	DBengine DB;
	DB.open("animals");
	DB.close();
	DB.open("humans");
	DB.close();
	DB.select("animals");
	//DB.output();
	DB.write("anials");
	DB.select("humans");
	//DB.output();
	DB.show("humans.db");
	DB.write("humans");
	return 0;
}