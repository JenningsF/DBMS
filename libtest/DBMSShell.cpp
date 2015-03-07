#include <string.h>
#include <iostream>
#include <fstream>
#include "DbmsApi.h"
using namespace std;

int main(int argc, char* argv[]) {
   if (argc > 1 && strcmp(argv[1],"-f") == 0) {
      if (argc != 3) {
         cout << "Invalid setup. Aborting...\n";
         return 1;
      }
      
      cout << "FILE: " << argv[2] << endl;
      ifstream testFile(argv[2]);
      if (!testFile.is_open()) {
         cout << "Failed to locate file. Aborting...\n";
         return 1;
      }
      
      //Get each line and parse
      string line;
      while (true) {
         getline(testFile, line);
         if (testFile.eof()) {
            return 0;
         }
         if (!DBMSParse(line)) {
			cout << getLastDBMSErrorDetail() << endl;	
		 }
      }
   } else {
      string query;
      while (true) {
         getline(cin, query);
         if (query.compare("quit") == 0) {
            return 0;
         }
         if (!DBMSParse(query)) {
			cout << getLastDBMSErrorDetail() << endl;	
		 }
      }
   }
}