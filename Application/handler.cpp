/*****************************************
Includes
Handler for DBMS:
Holds all functions for running queries and commands
Zachary Varnadore
Chad Young

*****************************************/

#include <iostream>
#include <vector>
#include "RelationsRegistry.h"
#include "handler.h"
#include <algorithm>
#include <fstream>
#include <cctype>

using namespace std;

/*****************************************
Global Structures
*****************************************/

RelationsRegistry relationContainer;
RelationsRegistry viewContainer;


/*****************************************
Utilities
 To perform Union or Difference each table needs to have
 the same number and types of columns
*****************************************/

bool isUnionCompatible(Table t1, Table t2) {
    if (t1.numberOfAttributes() != t2.numberOfAttributes())
        return false;
    for (int i=0; i < t1.numberOfAttributes(); ++i) {
        if (t1.getAttribute(i).type != t2.getAttribute(i).type)
            return false;
    }

    return true;
}

vector<bool> doAllOperations(string firstOperand, string secondOperand){
   vector<bool> resultsVector(6);
   resultsVector[0] = (firstOperand.compare(secondOperand) == 0);
   resultsVector[1] = (firstOperand.compare(secondOperand) != 0);
   resultsVector[2] = (firstOperand.compare(secondOperand) == 0 
                        || firstOperand.compare(secondOperand) > 0);
   resultsVector[3] = (firstOperand.compare(secondOperand) == 0 
                        || firstOperand.compare(secondOperand) < 0);
   resultsVector[4] = (firstOperand.compare(secondOperand) > 0);
   resultsVector[5] = (firstOperand.compare(secondOperand) < 0);
   return resultsVector;
}

vector<bool> doAllOperations(int firstOperand, int secondOperand){
   vector<bool> resultsVector(6);
   resultsVector[0] = (firstOperand == secondOperand);
   resultsVector[1] = (firstOperand != secondOperand);
   resultsVector[2] = (firstOperand >= secondOperand);
   resultsVector[3] = (firstOperand <= secondOperand);
   resultsVector[4] = (firstOperand > secondOperand);
   resultsVector[5] = (firstOperand < secondOperand);
   return resultsVector;
}

vector<bool> doAllOperations(double firstOperand, double secondOperand){
   vector<bool> resultsVector(6);
   resultsVector[0] = (firstOperand == secondOperand);
   resultsVector[1] = (firstOperand != secondOperand);
   resultsVector[2] = (firstOperand >= secondOperand);
   resultsVector[3] = (firstOperand <= secondOperand);
   resultsVector[4] = (firstOperand > secondOperand);
   resultsVector[5] = (firstOperand < secondOperand);
   return resultsVector;
}


/*****************************************
Conditions Objects Implementations
*****************************************/
bool Comparison::runComparison(Table view, int index){
   if(op == none)    //only run the condition or the operation
      return condition.runCondition(view, index);
   //else operand op operand
   firstOperand.setType(view);
   secondOperand.setType(view);
//if both operand are digits, change all to doubles, call doAllOperations 
 if((firstOperand.type == 1 || firstOperand.type == 2) && 
		secondOperand.type == 1 || secondOperand.type == 2){
	double first = stod(firstOperand.getValue(view, index));
	double second = stod(secondOperand.getValue(view, index));
	return doAllOperations(first, second)[op];
   }
//if both operands are strings, call doAllOperations 
 else if(firstOperand.type == 3 || secondOperand.type == 3){
	string first = firstOperand.getValue(view, index);
	string second = secondOperand.getValue(view, index);
	return doAllOperations(first, second)[op];
   }
   else{
	//mismatched types
	throw BadComparison();
   }
  
   
   
}

//Runs && function on one or more comparisons
bool Conjunction::runConjunction(Table view, int index){
    for(int i = 0; i < comparisonList.size(); ++i){
        if(!comparisonList[i].runComparison(view, index)){
        return false;
        }
    }
    return true;
}

//runs || on one or more conjunctions
bool Condition::runCondition(Table view, int index){
    for(int i = 0; i < conjunctionList.size(); ++i){
        if(conjunctionList[i].runConjunction(view, index)){
        return true;
        }
    }
    return false;
}

//return value from table or the literal
string Operand::getValue(Table view, int index){
    if(isLiteral){
        return value;
    }
	int attrIndex = view.getAttributeIndex(value);
	if(attrIndex == -1){
		throw AttrDNE();
	}
     return view.getValue(view.getAttributeIndex(value), index);
 }
 
 //set operand type which is based on table attribute
 void Operand::setType(Table view){
	if(!isLiteral)
		type = view.getAttribute(view.getAttributeIndex(value)).type;
 }

/*****************************************
Query Structures Implementations
 Each functionality is built into its own struct
*****************************************/
//Determine which tuples to keep
Table Selection::run(){
   //gets base table to work with
   Table queryView = atomicExpression->run();

   for(int i = 0; i < queryView.size(); ++i){
      //only want tuples that pass condition
     if(!condition.runCondition(queryView, i)){
         queryView.removeTuple(i);
         --i;
      }
   }
   return queryView;
}


//returns either table or the result of an expression 
Table AtomicExpression::run(){
  if(expression != NULL){
      return expression->run();
   }
  
  else {
	  if (!relationContainer.hasTable(relationName)) {
		  return Table();
	  }
      return relationContainer.getTable(relationName);
   }
}
//determine which attribute (columns) to show
Table Projection::run(){
      Table queryView = atomicExpression->run();
	  
	   if(attributeList.size() > 0 ){ 
         vector<Attribute> queryAttr = queryView.getAttributeList();

		 for(int i = 0; i < queryAttr.size(); ++i){
 	    //remove all columns not specified
		if(find( attributeList.begin(),
                  attributeList.end(),
                  queryAttr[i].name
               )
               == attributeList.end()
            ){
                queryView.removeAttribute(i);
			   queryAttr = queryView.getAttributeList();
			   i--;
            }
         }
         return queryView;
      }
      else {
         return queryView; // If none were specified, return original view
      }
   }
   
   

Table Renaming::run(){
    Table queryView = atomicExpression->run();
	//if there are too many rename variables, remove the extras
	if(attributeList.size() > queryView.getAttributeList().size()){
		attributeList.resize(queryView.getAttributeList().size());
	}
	for(int i = 0; i < attributeList.size(); ++i){
		queryView.renameAttribute(i, attributeList[i]);
	}
	return queryView;
}


/*
adds all the tuples from the second table to the first table
tables must be compatible (same number of columns with matching column types
automatically removes duplicate rows
*/
Table Union::run(){
      Table queryViewOne = firstAtomicExpression->run();
      Table queryViewTwo = secondAtomicExpression->run();

        if (!isUnionCompatible(queryViewOne, queryViewTwo)){
			throw IncompatibleTables();
			return Table();
		}
      for(int i = 0; i < queryViewTwo.size(); ++i){
         queryViewOne.addTuple(queryViewTwo.getTuple(i));
      }
      return queryViewOne;
   }


/*
removes all tuples found in second table from the first table
tables must be compatible
*/
Table Difference::run(){

   Table queryViewOne = firstAtomicExpression->run();
   Table queryViewTwo = secondAtomicExpression->run();

   if (!isUnionCompatible(queryViewOne, queryViewTwo))
        return Table();

   int index = -1; //signifies Tuple not found
   for(int i = 0; i < queryViewTwo.size(); ++i){
      index = queryViewOne.getTupleIndex(queryViewTwo.getTuple(i));
      if(index != -1){
         queryViewOne.removeTuple(index);
      }
   }
   return queryViewOne;
}

/*
cross product on the tables, do not need to be compatible
new table name is tableOneName*tableTwoName
if there are duplicate columns they will be renamed tableOneName.columnName
and tableTwoName.columnName
*/
Table Product::run(){
	Table queryViewOne = firstAtomicExpression->run();
	Table queryViewTwo = secondAtomicExpression->run();
	
	vector<Attribute> newAttrList;
	for(int i = 0; i < queryViewOne.numberOfAttributes(); ++i){
		Attribute toPush = queryViewOne.getAttribute(i);
		//determine if other table has attr with the same name
		if(queryViewTwo.getAttributeIndex(toPush.name) != -1){
			toPush.name = queryViewOne.getTableName() + "." + toPush.name;
		}
		newAttrList.push_back(toPush);		
	}
	for(int i = 0; i < queryViewTwo.numberOfAttributes(); ++i){
		Attribute toPush = queryViewTwo.getAttribute(i);
		//determine if other table has attr with the same name
		if(queryViewOne.getAttributeIndex(toPush.name) != -1){
			toPush.name = queryViewTwo.getTableName() + "." + toPush.name;
		}
		newAttrList.push_back(toPush);		
	}
	string newTableName = queryViewOne.getTableName() + "*" + queryViewTwo.getTableName();
	Table viewToReturn(newTableName, newAttrList);
	for(int i = 0; i < queryViewOne.size(); ++i){ 
		for(int j = 0; j < queryViewTwo.size(); ++j){
			//need to combine each pair of tuples into one tuple
			vector<string> tupleFirstHalf = queryViewOne.getTuple(i);
			vector<string> tupleSecondHalf = queryViewTwo.getTuple(j);
			tupleFirstHalf.reserve(tupleFirstHalf.size() + tupleSecondHalf.size());
			tupleFirstHalf.insert(tupleFirstHalf.end(),
									tupleSecondHalf.begin(),
									tupleSecondHalf.end());
			//build a new view to hold all of the new tuples
			viewToReturn.addTuple(tupleFirstHalf);
		}
	}
	return viewToReturn;
}

Table Query::runQuery(){
   //initialize view called relationName and set to following command
   Table queryResult = expression->run();
   return queryResult;
}

/*****************************************
Commands Implementations
*****************************************/

bool writeDatabaseFile(string relationName) {
   //each table is held in a file named <relationName>.txt
   string fileName = relationName + ".txt";
   ofstream out(fileName);
   if(!out.is_open()){
	   throw IOFail();
      return false;
   }
   Table relation = relationContainer.getTable(relationName);
   vector<Attribute> attrList = relation.getAttributeList();
   //write number of columns
   out << attrList.size() << endl;
   //table data deliminated by '~'
   //write out table attributes
   for(int i = 0; i < attrList.size(); ++i){
      out << attrList[i].name << "~";
      out << attrList[i].type << "~";
      out << attrList[i].key << endl;
   }
   vector<string> data;
   //write out tuples
   for(int i =0; i < relation.size(); ++i){
      data = relation.getTuple(i);
      out << data[0];
      for(int j = 1; j < data.size(); ++j){
         out << "~" << data[j];
      }
      if(i < relation.size() -1)
         out << endl;
   }
   out.close();
   return true;
}

bool openDatabaseFile(string relationName) {
   //each table is held in a file named <relationName>.txt
   ifstream in(relationName + ".txt");
   if(!in.is_open()){
      //throw IONoFile();
      return false;
   }

   string currentLine;
   //read number of columns
   getline(in, currentLine);
   int numAttributes = atoi(currentLine.c_str());
   vector<Attribute> attrList;
   string name;
   string type;
   string key;
   //read attr data
   for(int i = 0; i < numAttributes; ++i){
      getline(in, name, '~');
      getline(in, type, '~'),
      getline(in, key);
      Attribute attrToPush(
         name,
         (dataType)atoi(type.c_str()),
         key.compare("1") == 0
      );
      attrList.push_back(attrToPush);
   }

   Table tableToAdd(relationName, attrList);
   //read tuples
   while(!in.eof()){
      vector<string> tupleToPush;
      for(int i =0; i < numAttributes - 1; ++i){
         getline(in, currentLine, '~');
         tupleToPush.push_back(currentLine);
      }
      getline(in, currentLine);
      tupleToPush.push_back(currentLine);
      tableToAdd.addTuple(tupleToPush);
   }
   relationContainer.addTable(relationName, tableToAdd);
   return true;
}


bool closeDatabaseFile(string relationName){
   writeDatabaseFile(relationName);
   relationContainer.removeTable(relationName);
   return true;
}

bool showTable(string relationName){
   if (!relationContainer.hasTable(relationName)) {
      return false;
   }
   relationContainer.getTable(relationName).printTable();
   return true;
}

//creates empty table
bool createTable(string relationName, vector<Attribute> attrList){
   Table tableToAdd(relationName, attrList);
   relationContainer.addTable(relationName, tableToAdd);
   return true;
}

void exitDBMS() {
    vector<string> allTables = relationContainer.getAllTableNames();
    for (int i=0; i<allTables.size(); ++i) {
        closeDatabaseFile(allTables[i]);
    }
}

//delete tuples that satisfy condition
void deleteValues(string relationName, Condition condition) {
    Table& updateTable = relationContainer.getTableReference(relationName);

    for(int i=0; i<updateTable.size(); ++i){
        if(condition.runCondition(updateTable, i)){
            updateTable.removeTuple(i);
            --i;
        }
    }
}

//update tuple values based on condition
void updateTable(
   string relationName,
   vector<StrPair> newVals,
   Condition condition)
{
    Table& updateTable = relationContainer.getTableReference(relationName);
    for(int i=0; i<updateTable.size(); ++i){
        if(condition.runCondition(updateTable, i)){
            for (int j=0; j<newVals.size(); ++j) {
               updateTable.updateTuple(i, newVals[j].str1, newVals[j].str2);
            }
        }
    }
}

//add tuple to table
void insertValues(string relationName, vector<string> values) {
	if (!relationContainer.hasTable(relationName))
		return;
    relationContainer.getTableReference(relationName).addTuple(values);
}

void insertValuesFromRelation(string relationTarget, const Table &source) {
   Table& temp = const_cast<Table&>(source);
   if (!relationContainer.hasTable(relationTarget)) {
      return;
   }
   Table& target = relationContainer.getTableReference(relationTarget);
   for (int i=0; i<temp.size(); ++i) {
      target.addTuple(temp.getTuple(i));
   }
}

Table getRelationCopy(string relation) {
   return relationContainer.getTable(relation);
}