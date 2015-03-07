/*****************************************
Includes
*****************************************/

#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;


/*****************************************
Parser Helper Function Definitions
*****************************************/

//removes all '('  ')' ',' and whitespace from front of a string
string cleanStringFront(string toClean){
	if (toClean.size() == 0) return string("");
	while(isspace(toClean[0]) || 
					toClean[0] == '(' || 
					toClean[0] == ')' ||
					toClean[0] == ','){
		toClean.erase(toClean.begin());	
	}
	return toClean;
}

//removes all '('  ')' ',' and whitespace from back of a string
string cleanStringBack(string toClean){
	if (toClean.size() == 0) return string("");
	while(isspace(toClean[toClean.size()-1])|| 
				toClean[toClean.size()-1] == '(' || 
				toClean[toClean.size()-1] == ')' ||
				toClean[toClean.size()-1] == ','){	
		toClean.erase(toClean.begin() + toClean.size() - 1);	
	}
	return toClean;
}

//removes all '('  ')' ',' and whitespace from front and back of a string
string cleanString(string toClean){
	return cleanStringFront(cleanStringBack(toClean));
}


//removes only front and back parenthesis
string removeOutsideParenth(string toClean){
	if (toClean.size() == 0) return string("");
	
	while(isspace(toClean[0]) || toClean[0] == '('){
		if(toClean[0] == '('){
			toClean.erase(toClean.begin());
			break;
		}
		toClean.erase(toClean.begin());	
	}	
	
	while(isspace(toClean[toClean.size()-1])|| toClean[toClean.size()-1] == ')'){	
		if(toClean[toClean.size()-1] == ')'){
			toClean.erase(toClean.begin() + toClean.size() - 1);	
			break;
		}
		toClean.erase(toClean.begin() + toClean.size() - 1);	
	}

	return toClean;
}

//returns location of close parenthesis that matches open paren.
int findCloseParenth(string query){
	int openParenth = 0;
	for(int i = 0; i < query.size(); ++i){
		if(query[i] == ')'){
			if(openParenth ==  1){
				return i;
			}
			else{
					openParenth--;
			}
		}
		else if(query[i] == '('){
			openParenth++;
		}
	}
	return -1; //signifies failure
}

//split query into two strings, used for project, rename, and select
bool splitQuery(string query, string &first, string &second){
	int splitLocation = findCloseParenth(query);
	splitLocation++;
	first = query.substr(0, splitLocation);
	second = query.substr(splitLocation);
	//remove extra parenthesis from front and back
	first = cleanString(first);
	second = cleanString(second);
	return true;
}

//checks if string has parenthesis
bool hasParenth(string query){
	for(int i = 0; i < query.size(); ++i){
		if(query[i] == '('){
			return true;
		}
		else if(isalnum((int)query[i])){
			return false;
		}
	}
	//throw error
}

//return location of operation
int findOperation(string query){
	for(int i = 0; i < query.size(); ++i){
		if(query[i] == '+' || 
			query[i] == '-' || 
			query[i] == '*'){
				return i;
		}
	}
	return -1; //signifies operation not found
}


//splits query into two strings, used for product, union, and difference
int splitQuery(string query, string &first, string &second, int &opInt){
   char operation;
   //skip any operations inside parenthesis 
   if(hasParenth(query)){
      splitQuery(query, first, second);
      //No op found
      if (findOperation(second) == -1) {
         operation = '4';
      }else {
         //update opInt and remove opInt char from second string
         operation = second[findOperation(second)];
         second = second.substr(findOperation(second)+1);
      }
   }
   //assume simple split with two relation names and one op
   else{
      first = query.substr(0, findOperation(query));
      second = query.substr(findOperation(query)+1);
	  int findOp = findOperation(query);
	  if ( findOp == -1 ) {
		  operation = '4';
	  } else {
		operation = query[findOp];
	  }
   }
   //update opInt
   switch (operation){
   case '+':
      opInt = 1;
      break;
   case '-':
      opInt = 2;
      break;
   case '*':
      opInt = 3;
      break;
   default:
      opInt = 4;
   }
    first = removeOutsideParenth(first);
    second = removeOutsideParenth(second);
   return 0;
}
