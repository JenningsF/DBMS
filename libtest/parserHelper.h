#ifndef PARSER_HELPER_H
#define PARSER_HELPER_H

/*****************************************
Includes
*****************************************/

#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;


/*****************************************
Data Structs
*****************************************/

class BadParse {};   //Thrown for bad parse strings



/*****************************************
Parser Helper Function Declarations
*****************************************/

//removes all white space and parenthesis from the beginning of the string
string cleanStringFront(string toClean);

//removes all white space and parenthesis from the beginning of the string
string cleanStringBack(string toClean);

//Calls both cleanStringFront and cleanStringBack
string cleanString(string toClean);

//removes only the outside set of parenthesis 
string removeOutsideParenth(string toClean);

//returns location of end of first parenthesis group
int findCloseParenth(string query);

//splits query into two strings, assumes perfect parenthesis 
bool splitQuery(string query, string &first, string &second);

//true is starts with open parenthesis, ignores whitespace
bool hasParenth(string query);

//returns location of first '+' '-' or '*'
int findOperation(string query);


//split query for union, difference, product. Also updates opInt which signals 
//the function type.
int splitQuery(string query, string &first, string &second, int &opInt);

#endif