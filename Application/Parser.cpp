/*****************************************
Includes
*****************************************/

#include <iostream>
#include "handler.h"
#include "queryparser.h"
#include "parserHelper.h"
#include "DbmsApi.h"
#include <sstream>
#include <string>
#include <stdexcept> 

using namespace std;

int lastDbmsError;
string errorDetail;

/*****************************************
Parser
*****************************************/

/*
Parses a string for the first literal and remove its from the string.
Arguments: a string containing literal(s).
Returns: A string containing the extracted literal.
*/
string parseLiteral(string &str) {
   int ind=0;
   while (str[ind] == ' ') {   //Remove whitespace at front
      ++ind;
   }
   
   if (ind>str.size()) {   //Make sure we are not at end (bad string)
      throw BadParse();
   }
   
   string toReturn = "";
   int find = ind;
   int nextFind = ind;
   while (true) {   //Locate delimiter ')' or ','
      nextFind = str.find(',', find);
      if (nextFind == -1) {
         nextFind = str.find(')', find);
         if (nextFind == -1) {   //No delimiter found 
            throw BadParse();
         }
         find = nextFind;
         break;
      } else {
         find = nextFind;
         if (str[nextFind-1] != '\\') {   //Check if comma is backslashed
            break;
         } 
      }
   }
   toReturn = str.substr(ind, (find-ind));   //Extract literal
   /* Clean literal if it is a string literal
   if (toReturn[0] == '\"' || toReturn[toReturn.size()-1] == '\"') {
      if (toReturn[0] != '\"') {
         throw BadParse();
      }
      if (toReturn[toReturn.size()-1] != '\"') {
         throw BadParse();
      }
      toReturn = cleanStringBack(toReturn);
      toReturn.erase(0,1);
      toReturn.erase(toReturn.size()-1, 1);
   }*/
   toReturn = cleanStringBack(toReturn);
   return toReturn;
}

/* Identifies a parsed attribute's data type */
dataType identifyType(string type)
{
	if(type == "INTEGER") return Integer;
	else if(type == "DOUBLE") return Double;
	else if(type == "VARCHAR") return Varchar;
	//else woops
}

/*
Primary parsing function that takes a query or command DBMS string, parses
it, and performs the query/command execution by calling into the DBMS core.
Arguments: String containing a query or command for DBMS.
*/
void startParse(string parseStr)
{
   /* Check front of string for command keyword */
   istringstream iss(parseStr);
	string nextCommand;
   iss >> nextCommand;
	
   if(nextCommand.compare("OPEN") == 0) {   //OPEN command
      string relName;
      iss >> relName;
      if (!isAlphaNumeric(relName)) {
         throw BadParse();
      }
      
      if (!openDatabaseFile(relName)) {
         throw new IONoFile;
      }
   }
   
   else if(nextCommand.compare("CLOSE") == 0) {   //CLOSE command
      string relName;
      iss >> relName;
      if (!isAlphaNumeric(relName)) {
         throw BadParse();
      }
      
      closeDatabaseFile(relName);
   }
   
   else if(nextCommand.compare("WRITE") == 0) {   //WRITE command
      string relName;
      iss >> relName;
      if (!isAlphaNumeric(relName)) {
         throw BadParse();
      }
      
      if (!writeDatabaseFile(relName)) {
         throw new IOFail;
      }
   }
   
   else if(nextCommand.compare("EXIT") == 0) {   //EXIT command
      exitDBMS();
   }
   
   else if(nextCommand.compare("SHOW") == 0) {   //SHOW command
	  if (parseStr.find('(') != -1) {
			string atExpr = parseStr.substr(parseStr.find("(")+1);
			atExpr = atExpr.substr(0, atExpr.size()-1);
			parseQuery(atExpr)->run().printTable();
	  } else {
		  string rname;
		  iss >> rname;
		  showTable(rname);
	  }
   }
   
   else if(nextCommand.compare("INSERT") == 0) {   //INSERT command
      string cur;
      iss >> cur;
      if (cur.compare("INTO") != 0) {
         throw BadParse();
      }
      
      //Extract relation name
      string rname;
      iss >> rname;
      if (!isAlphaNumeric(rname)) {
         throw BadParse();
      }
      
      iss >> cur;
      if (cur.compare("VALUES") !=0) {
         throw BadParse();
      }
      
      iss >> cur;
      if (cur.compare("FROM") !=0) {
         throw BadParse();
      }
      
      /* Check which kind of INSERT we have and execute */
      iss >> cur;
      if (cur.compare("RELATION") != 0) {   //INSERT from VALS command
         if (cur.find('(') == -1) {
            throw BadParse();
         }
         /* Parse each literal and add to vector */
         vector<string> newTuple;
         string literal = parseStr.substr(parseStr.find('(')+1);   //Clean front
         while (literal[0] != ')') {
            string nextLit = parseLiteral(literal);
            newTuple.push_back(nextLit);
            if (literal.find(',') == -1) {   //No more literals
               break;
            } else {
               literal = literal.substr(literal.find(',')+1);
            }
         }
         
         insertValues(rname, newTuple);   //Add parsed literals to relation
      }
      else if (cur.compare("RELATION") == 0) {   //INSERT from RELATION command
         string expr;
         iss >> expr;   //Extract the expression
         if (expr.find('(') == -1) {
            throw BadParse();
         }
         expr = expr.substr(expr.find('('));   //Extract the expression
         insertValuesFromRelation(rname, parseQuery(expr)->run());   //execute
      }
      
   }
   
   else if(nextCommand.compare("DELETE") == 0) {   //DELETE command
      string cur;
      iss >> cur;
      if (cur.compare("FROM") != 0) {
         throw BadParse();
      }
      
      string rname;
      iss >> rname;   //Extract relation name
      if (!isAlphaNumeric(rname)) {   //Check for valid name
         throw BadParse();
      }
      
      iss >> cur;
      if (cur.compare("WHERE") != 0) {
         throw BadParse();
      }
      
      string cond;
      if (parseStr.find('(') == -1) {
         throw BadParse();
      }
      /* Extract condition string and parse it */
      cond = parseStr.substr(parseStr.find('('));
      deleteValues(rname, parseCondition(cond));   //Run DELETE
   }
   
   else if(nextCommand.compare("CREATE") == 0){
      iss >> nextCommand;
      char nextChar;   //used to check the next char
      
      if(nextCommand.compare("TABLE") != 0){
         //ERROR
      }
      
      vector<Attribute> readAttributes;   //stores new Attributes read in
      string tableName;   //used to read in the name of the Table
      
      //parse potential whitespace
      iss.get(nextChar);
      while(nextChar == ' '){
         iss.get(nextChar);
      }
      //read Table name
      while(true){
         if(nextChar == ' ') break;
         else if(nextChar == '(') break;
         else{
            tableName += nextChar;
            iss.get(nextChar);
         }
      }
      if(tableName.size() == 0); //error, no name
      
      //parse potential whitespace
      while(nextChar == ' '){
         iss.get(nextChar);
      }
      if(nextChar != '('); //syntax error
      
      //read Attributes
      while(true){
         string attrName;   //new Attribute's name
         dataType attrType;   //new Attribute's type
         int attrSize;      //used to read in the size of the Attribute//////////////////////////add feature
         
         iss >> attrName;   //read in Attribute's name
         
         string readType;   //used to read in the next string which
                        //contains data about the Attribute's type and size
         
         //read in the Attribute's type
         iss.get(nextChar);
         while(nextChar == ' '){
            iss.get(nextChar);
         }
         while(isupper(nextChar)){
            readType += nextChar;
            iss.get(nextChar);
         }
         //Find the type of the Attribute
         attrType = identifyType(readType);
         
         if(attrType == Varchar){
            if(nextChar == '('){
               //Find the size of the Attribute
               iss >> attrSize;//////////////////////////////////////////////////////////////add feature
               iss.get(nextChar);
               //parse potential whitespace and remove ')'
               while(nextChar == ' '){
                  iss.get(nextChar);
               }
               if(nextChar == ')'){
                  iss.get(nextChar);
               }
               //else syntax error
            }
            //else syntax error
         }
         readAttributes.push_back(Attribute(attrName, attrType));   //add Attribute
         
         //parse potential whitespace and check if there are more Attributes to read
         while(nextChar == ' '){
            iss.get(nextChar);
         }
         if(nextChar == ')') break;
         else if(nextChar == ',') continue;
         else; //syntax error
      }
      
      //read in the primary keys of the table
      iss >> nextCommand;
      if(nextCommand.compare("PRIMARY") == 0){
         iss >> nextCommand;
         if(nextCommand.compare("KEY") != 0){
            //ERROR
         }
         
         //parse potential whitespace
         iss.get(nextChar);
         while(nextChar == ' '){
            iss.get(nextChar);
         }
         if(nextChar != '('); //syntax error
         
         //loop for reading in Primary Keys
         while(true){
            string newPrime;   //name of new Primary Key
            
            //parse potential whitespace
            iss.get(nextChar);
            while(nextChar == ' '){
               iss.get(nextChar);
            }
            
            //read name of Primary Key
            while(true){
               if(nextChar == ' ') break;
               else if(nextChar == ',') break;
               else if(nextChar == ')') break;
               else{
                  newPrime += nextChar;
                  iss.get(nextChar);
               }
            }
            
            //convert correct Attributes to Primary Attributes
            for(int i =0; i < readAttributes.size(); ++i){
               if(readAttributes[i].name == newPrime){
                  readAttributes[i].key = true;
                  break;
               }
               else if(i == readAttributes.size() - 1); //ERROR, pKey not an Attribute
            }
            
            //parse potential whitespace
            while(nextChar == ' '){
               iss.get(nextChar);
            }
            
            //check if there are more primary keys to read
            if(nextChar == ')') break;
            else if(nextChar == ',') continue;
            else; //syntax error
         }
         
         createTable(tableName, readAttributes);   //add new Table to Relations Registry
      }
      //else syntax error
   }
   
   else if(nextCommand.compare("UPDATE") == 0){
      //open Table
      string tableName;   //name of table
      vector<StrPair> attrValPairs;   //used to store pairs of Attributes and their values
      string valSection;   //section of the command arguments

      iss >> tableName;
      openDatabaseFile(tableName);

      iss >> nextCommand;
      if(nextCommand.compare("SET") != 0){
         throw BadParse();
      }
      
      //read in Attributes and their new values
      string attrToChange;   //name of Attribute that will be updated
      string newVal;      //new value of Attribute
      
      //store the rest of the command in valSection
      string line;
      getline(iss, line, ';');
      while(line.substr((line.size()-1)).compare("\\") == 0){
         line.pop_back();
         valSection += line;
         valSection += ';';
         getline(iss, line, ';');
      }
      valSection += line;

      //read in new value of Attribute
      bool moreAttr = true;
      while(moreAttr){
         //read in Attribute name
         attrToChange = valSection.substr(0, valSection.find('='));
         attrToChange = cleanString(attrToChange);
         valSection = valSection.substr(valSection.find('=')+1);
         
         while(true){
            if(valSection.find(',') == -1){
               if(valSection.rfind("WHERE") == -1){
                  throw BadParse();
               }
               else{
                  newVal += valSection.substr(0, valSection.rfind("WHERE"));
                  moreAttr = false;
                  break;
               }
            }
            else{
               newVal += valSection.substr(0, valSection.find(',')+1);
               valSection = valSection.substr(valSection.find(',')+1);
               if((newVal.substr((newVal.size()-3), 3)).compare("\\,")){
                  newVal.erase(newVal.end()-2);
                  continue;
               }
               else {
                  break;
               }
            }
         }
         
         //add pair of Attribute and value to vector
         newVal = cleanString(newVal);
         if(newVal.front() == '"'){
            newVal.erase(newVal.begin());
         }
         if(newVal.back() == '"'){
            newVal.pop_back();
         }
         newVal = cleanString(newVal);
         attrValPairs.push_back(StrPair(attrToChange, newVal));
      }
      //reduce valSection so that it contains only the Condition
      valSection = valSection.substr(valSection.rfind("WHERE") + 6);
      valSection = cleanString(valSection);
      
      //update the table
      updateTable(tableName, attrValPairs, parseCondition(valSection));
   }
   
   else {   //A query
      string check;
      iss >> check;
      if (check.compare("<-")) {   //Syntax check
         throw BadParse();
      }
      parseStr = parseStr.substr(parseStr.find("<-")+3);
      Table t = parseQuery(parseStr)->run();

	  createTable(nextCommand, t.getAttributeList());
	  insertValuesFromRelation(nextCommand, t);
   }
}

bool DBMSParse(string dbmsStr) {
   try {
      startParse(dbmsStr);
   } catch (BadParse &e) {
      lastDbmsError = badParse;
      errorDetail = "Invalid query/command syntax.";
	   return false;
   } catch (AttrDNE &e) {
      lastDbmsError = noAttribute;
      errorDetail = "Attribute specified does not exist.";
	   return false;
   } catch (NoSuchTable &e) {
      lastDbmsError = noTable;
      errorDetail = "Relation specified does not exist.";
	   return false;
   } catch (IOFail &e) {
      lastDbmsError = ioFailure;
      errorDetail = "Unexpected IO failure occurred.";
	   return false;
   } catch (IONoFile &e) {
      lastDbmsError = ioNoFile;
      errorDetail = "Specified table has no file found.";
	   return false;
   } catch (BadComparison &e) {
      lastDbmsError = incompatibleTypes;
      errorDetail = "Values specified do not have matching types for comparison.";
      return false;
   } catch (const std::invalid_argument &e) {
      lastDbmsError = badType;
      errorDetail = "Values specified do not conform to defined types for relation.";
	   return false;
   } catch (BadType &e) {
      lastDbmsError = badType;
      errorDetail = "Values specified do not conform to defined types for relation.";
	   return false;
   } catch (IncompatibleTables &e) {
      lastDbmsError = incompatibleTables;
      errorDetail = "Relation attributes are not compatible for operation.";
	  return false;
   }

   return true;
}

int getLastDBMSError() {
	return lastDbmsError;
}

string getLastDBMSErrorDetail() {
	return errorDetail;
}

Table GetRelation(string relation) {
   return getRelationCopy(relation);
}
