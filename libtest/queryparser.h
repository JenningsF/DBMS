#ifndef QUERY_PARSER_H
#define QUERY_PARSER_H

/*****************************************
Includes
*****************************************/

#include <sstream>
#include <string>
#include <ctype.h> //isalnum()
#include "parserHelper.h"
#include "handler.h"
#include "Table.h"
using namespace std;


/*****************************************
Query Parsing Function Declarations
*****************************************/

/*
Takes the query string (minus the view name) and builds corresponding query
structs.
Returns: a pointer to an expression type which can be a projection, selection,
rename, union, difference, or product.
*/
Expression* parseQuery(string toParse);

/*
Parses a string as a Condition and returns the parsed Condition.
*/
Condition parseCondition(string str);

/*
Parses a string as Conjunction (a list) and returns the parsed Conjunction.
*/
Conjunction parseConjunction(string str);

/*
Parses a string as a Comparison, which is a (operand OP operand) format or
just a relation name. Returns the parsed Comparison.
*/
Comparison parseComparison(string str);

/*
Parses a string as an Operand to determine if it is a literal or attribute
name. Returns the parsed Operand.
*/
Operand parseOperand(string toParse);

/*
Parses the comparison string to determine the comparison operation specified.
Returns on of the following values corresponding to an operation:
0:'=', 1:'!=', 2:'>=', 3:'<=', 4:'>', 5:'<', -1:not found
*/
int findOp(string comparison, int &op);


/*****************************************
Query Parsing Helper Function Declarations
*****************************************/

/*
Removes all spaces from given string to make parsing easier.
*/
void replaceAllSpaces(string &str);

/*
Returns true if given string only has alphanumeric characters, false otherwise.
*/
bool isAlphaNumeric(string str);

/*
Converts a string list (comma delim) to a vector of strings
*/
vector<string> stringListToVector(string toVector);

#endif