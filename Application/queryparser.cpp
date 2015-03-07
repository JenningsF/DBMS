#include "queryparser.h"

/*****************************************
Query Parsing Function Definitions
*****************************************/

Expression* parseQuery(string toParse) {
   //remove trailing semicolon if nec
   if(toParse[toParse.size()-1] == ';'){
		toParse.pop_back();
   }
   istringstream iss(toParse);

   //Check front of query string for keyword
   string front;
   if (!(iss >> front)) {   //Throw BadParse if we have an empty string
      throw BadParse();
   }
   
   //Check if front is query keyword (project, select, rename):
   //Projection build
   if (front.compare("project") == 0) {

      string first, second;
      splitQuery(toParse.substr(toParse.find('(')), first, second);
      vector<string> attrs = stringListToVector(first);	 
	 
      Projection *proj = new Projection;
      proj->attributeList = attrs;
      proj->atomicExpression = (AtomicExpression*)parseQuery(second);
      return proj;
   }

	//Selection build
   else if (front.compare("select") == 0) {
      string first, second;
      splitQuery(toParse.substr(toParse.find('(')), first, second);
      Selection *select = new Selection;
      select->condition = parseCondition(first);
      select->atomicExpression = parseQuery(second);
      return select;
   } 
   
   //Rename build   
   else if (front.compare("rename") == 0) {
      string first, second;
      splitQuery(toParse.substr(toParse.find('(')), first, second);
      vector<string> newNames = stringListToVector(first);
      Renaming *rename = new Renaming;
      rename->attributeList = newNames;
      rename->atomicExpression = parseQuery(second);
      return rename;
   }
   
   //Else: expect a difference, union, product OR just relation name
   string first;   //Will be: relation name or argument list
   string second;   //Will be: a query to further parse or relation name
   int op;
   
   //Parse (expected) operation string
   splitQuery(toParse, first, second, op);
 
   //Check to see if we got an operation or just a relation name
   switch(op) {
      case(1): {   //Union
         Union *uni = new Union;
         uni->firstAtomicExpression = parseQuery(first);
         uni->secondAtomicExpression = parseQuery(second);
         return uni;
         break;
		}
      case(2): {  //Difference
         Difference *diff = new Difference;
         diff->firstAtomicExpression = parseQuery(first);
         diff->secondAtomicExpression = parseQuery(second);
         return diff;
         break;
		}
      case(3): {   //Product
         Product *prod = new Product;
         prod->firstAtomicExpression = parseQuery(first);
         prod->secondAtomicExpression = parseQuery(second);
         return prod;
         break;
		}
      case(4): {	  //No-op (relation name usually)
         AtomicExpression *atm = new AtomicExpression;
         atm->relationName = cleanString(first);
         atm->expression = NULL;
         return atm;
         break;
		}
   }
}

Condition parseCondition(string str) {
   Condition cond;
   int splitLocation = str.find("||");
   string first, second;
	//if || is found, split into two conjunctions
   if(splitLocation != -1){
      first = str.substr(0, splitLocation);
      second = str.substr(splitLocation + 2);
      cond.conjunctionList.push_back(parseConjunction(first));
      cond.conjunctionList.push_back(parseConjunction(second));
   } else {    //treat as a single conjunction
		cond.conjunctionList.push_back(parseConjunction(str));
	}
	return cond;
}

Conjunction parseConjunction(string str) {
   Conjunction conj;
    int splitLocation = str.find("&&");
   string first, second;
   //found &&, split into two comparisons
   if(splitLocation != -1){
      first = str.substr(0, splitLocation);
      second = str.substr(splitLocation + 2);
      conj.comparisonList.push_back(parseComparison(first));
      conj.comparisonList.push_back(parseComparison(second));
   } else {   //no &&, treat as a singer comparison
		conj.comparisonList.push_back(parseComparison(str));
	}
	return conj;
}

Comparison parseComparison(string toParse){
	Comparison comp;
	//if neither && or || are found, parse operand op operand format
	if(toParse.find("&&") == -1 && toParse.find("||") == -1){
		//operand op operand format
		int opLocation = findOp(toParse, comp.op);
		comp.firstOperand = parseOperand(toParse.substr(0, opLocation));
		string operandString = toParse.substr(opLocation + 1);
		if(operandString[0] == '='){
			operandString.erase(0,1);
		}
		comp.secondOperand = parseOperand(operandString);
	} else {   // && or || was found, must be a new condition
		comp.condition = parseCondition(toParse);
		comp.op = none;
	}
	return comp;
}

Operand parseOperand(string toParse){
	toParse = cleanString(toParse);
	Operand operand;
	operand.isLiteral = false;
	//if it starts with quotes or a num, it is a literal
	if(toParse[0] == '"'){
		toParse.erase(0, 1);
		toParse.erase(toParse.size()-1, 1);
		operand.isLiteral = true;
		operand.type = Varchar;
	}
	if(isdigit(toParse[0])){
		operand.isLiteral = true;
		operand.type = Double;
	}	
	operand.value = toParse;
	return operand;
}

//finds the operation inside a comparison. returns location, updates op which is passed by reference
int findOp(string comparison, int &op){
	for(int i =0; i < comparison.size(); ++i){
		switch(comparison[i]){
			case '=': 
				op = 0;
				return i;
				break;
			
			case '!': 
				op = 1;
				return i;
				break;
			
			case '>': 
				if(comparison[i+1] == '='){
					op = 2;
					return i;
						
				}
				op = 4;
				return i;
				break;
            
			case '<': 
				if(comparison[i+1] == '='){
					op = 3;
					return i;						
				}
				op = 5;
				return i;
				break;
			
			default:
				op = -1;
				break;
		}		
	}
	return -1;
}


/*****************************************
Query Parsing Helper Function Definitions
*****************************************/

bool isAlphaNumeric(string str) {
   int i;
   for (i=0; i<str.size(); ++i) {
      if (!isalnum(str[i])) {
         return false;
      }
   }
   return true;
}

void replaceAllSpaces(string &str) {
   int ind = -1;
   ind = str.find(" ");
   while (ind != -1) {
      str.erase(ind, 1);
      ind = str.find(" ");
   }
}

vector<string> stringListToVector(string toVector){
	toVector = cleanString(toVector);
	istringstream paramsExtract(toVector);
   string extract="";
   vector<string> attrs;  
   while(paramsExtract >> extract) {  
      extract = cleanString(extract);
      attrs.push_back(extract);

      extract="";
      //remove extra commas and spaces
      while(paramsExtract.peek() == ',' || paramsExtract.peek() == ' '){
         paramsExtract.ignore();
      }
   }
	return attrs;
}