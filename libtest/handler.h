#ifndef HANDLER_H
#define HANDLER_H

/*****************************************
Includes
*****************************************/

#include <iostream>
#include <vector>
#include <memory>
#include "Table.h"


class AttrDNE {};
class BadComparison{};
class NoSuchTable {};   //Thrown when a table is referenced that doesn't exist
class IncompatibleTables {};

/*****************************************
Utilities
*****************************************/

enum Operation {
   equal = 0,
   notEqual = 1,
   greaterEqual = 2,
   lessEqual = 3,
   greater = 4,
   less = 5,
   none = -1
};

struct StrPair {
   string str1;
   string str2;
   
   StrPair(string s1, string s2): str1(s1), str2(s2) {}
};

struct Operand;
struct Comparison;
struct Conjunction;
struct Condition;

/*
   The following doAllOperation functions does each compare operator on
   strings, integers, and doubles respectively. The compare results are
   stored and returned in a vector.
*/
vector<bool> doAllOperations(string firstOperand, string secondOperand);

vector<bool> doAllOperations(int firstOperand, int secondOperand);

vector<bool> doAllOperations(double firstOperand, double secondOperand);


/*****************************************
Conditions Structures
*****************************************/
//part of a comparison
struct Operand {
   string value;
   dataType type;
   bool isLiteral;
   string getValue(Table view, int index);
   void setType(Table view);
};

//series of conjuctions, binded by || (OR)
struct Condition {
   vector<Conjunction> conjunctionList;
   bool runCondition(Table view, int index);
};

//either another condition or an Operand Operation Operand format
struct Comparison{
   Operand firstOperand;
   Operand secondOperand;
   int op;
   Condition condition;

   bool runComparison(Table view, int index);
};

//series of comparison, binded by && (AND)
struct Conjunction{
   vector<Comparison> comparisonList;
   bool runConjunction(Table view, int index);
};

class IOFail {};   //Thrown for IO issues
class IONoFile {};   //Thrown when read file not found


/*****************************************
Query Structures
*****************************************/
/*
   -This struct defines an expression as an abstract class.
   -Inherited by: AtomicExpression, Selection, Projection, Renaming, Union,
    Difference, Product.
*/
struct Expression {
   virtual Table run()=0;
};

/*
   -This struct defines an atomic expression. An AtomicExpression either
    contains just a relation name or an expression.
   -run() returns a Table as a result of the AtomicExpression.
*/
struct AtomicExpression : public Expression {
   string relationName;
   Expression* expression;
  
   AtomicExpression(): expression(0) {}   //Default constructor
   Table run();
};

/*
   -This struct defines a selection query. A Selections contains a Condition
    for selection criteria and an AtomicExpression for an argument.
   -run() returns the result Table that contains tuples meeting the given
    conditions.
*/
struct Selection : public Expression {
   Condition condition;
   Expression *atomicExpression;
   ~Selection(){delete atomicExpression;}
  
  Table run();
};

/*
   -This struct defines a projection query. A Projection contains an
    AtomicExpression and a vector list of attribute names to project.
   -run() returns a result Table that contains the given attributes.
*/
struct Projection : public Expression {
   vector<string> attributeList;
   Expression *atomicExpression;
   ~Projection(){delete atomicExpression;}
   Table run();
};

/*
   -This struct defines a rename query. A Renaming contains an AtomicExpression
    and a vector list of renaming names as arguments.
   -run() returns the result Table that contains a Table with the given new
    new names.
*/
struct Renaming : public Expression {
   vector<string> attributeList;
   Expression *atomicExpression;
   ~Renaming(){delete atomicExpression;}
   Table run();
};

/*
   -This struct defines a union query. A union query contains two
    AtomicExpressions as arguments.
   -run() returns the Table that contains the union between result Tables of
    both AtomicExpressions.
*/
struct Union : public Expression {
   Expression *firstAtomicExpression;
   Expression *secondAtomicExpression;
   ~Union(){delete firstAtomicExpression; delete secondAtomicExpression;}

   Table run();
};

/*
   -This struct defines a union query. A union query contains two
    AtomicExpressions as arguments.
   -run() returns the Table that contains the union between result Tables of
    both AtomicExpressions.
*/
struct Difference : public Expression {
   Expression *firstAtomicExpression;
   Expression *secondAtomicExpression;
   ~Difference(){delete firstAtomicExpression; delete secondAtomicExpression;}

   Table run();
};

/*
   -This struct defines a cross product query. A product query contains two
    AtomicExpressions as arguments.
   -run() returns the Table that contains the cross product between result 
    Tables of both AtomicExpressions.
*/
struct Product : public Expression {
   Expression *firstAtomicExpression;
   Expression *secondAtomicExpression;
   ~Product(){delete firstAtomicExpression; delete secondAtomicExpression;}

   Table run();
};

/*
   -This struct defines a query object. A Query is the head of a tree of
    Expressions and AtomicExpressions that define an entire query.
*/
struct Query {
   string relationName;
  // unique_ptr<Expression> expression;
  Expression *expression;
  // Query(Expression *expr) {expression = unique_ptr<Expression>(expr);}
   Table runQuery();
   //Query(string query);
};



/*****************************************
Commands
*****************************************/

/*
   -Writes a Table in the RelationsRegister to a file.
   -Arguments: Name of Table to write.
   -Returns: Whether the write was successful.
*/
bool writeDatabaseFile(string relationName);

/*
   -Opens a database file and reads data into a Table to the RelationRegister.
   -Arguments: Name of Table to write.
   -Returns: Whether the write was successful.
*/
bool openDatabaseFile(string relationName);

/*
   -Closes a Table by removing it from the RelationRegister and writing it
    to disk.
   -Arguments: Name of Table to close.
   -Returns: Whether the close was successful.
*/
bool closeDatabaseFile(string relationName);

/*
   -Prints out the content of the table to cout.
   -Arguments: Name of Table to show.
   -Returns: Whether the show was successful.
*/
bool showTable(string relationName);

/*
   -Creates a new Table into the RelationRegister with the given Attributes.
   -Arguments: Name of Table to create and vector of Attributes to put in.
   -Returns: Whether the create was successful. The operation will fail if a
    Table with the same name already exists.
*/
bool createTable(string relationName, vector<Attribute> attrList);

/*
   -Closes all Tables in the RelationRegister and writes them to file.
*/
void exitDBMS();

/*
   -Prints out the content of the table to cout.
   -Arguments: Name of Table to show.
   -Returns: Whether the show was successful.
*/
void deleteValues(string relationName, Condition condition);

/*
   -Updates a RelationRegiter's Table's tuples that meet the Condition with given values.
   -Arguments: Name of Table to update, a vector of values to put in, and abort a
    Condition to select tuples to update.
   -Returns: Whether the update was successful.
*/
void updateTable(
                  string relationName, 
                  vector<StrPair> newVals, 
                  Condition condition
                );

/*
   -Adds the given tuple values as tuples to a Table in the RelationRegister.
   -Arguments: Name of Table to insert into, a vector of values to insert.
*/                
void insertValues(string relationName, vector<string> values);

/*
   -Copies tuple values to a Table in the RelationRegister from a given Table.
   -Arguments: Name of Table to insert into, a Table to retrieve values from.
*/ 
void insertValuesFromRelation(string relationTarget, const Table &source);

/* Returns a copy of specified relation for user */
Table getRelationCopy(string relation);

#endif