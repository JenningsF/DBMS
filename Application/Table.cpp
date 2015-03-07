/*****************************************
Includes
*****************************************/
#include "Table.h"


/*****************************************
Utilities
*****************************************/

bool isSame(vector<string> first, vector<string> second){
   if(first.size() != second.size())
      return false;
   for(int i = 0; i < first.size(); ++i){
      if(first[i] != second[i])
         return false;
   }
   return true;
}


/*****************************************
Table implementation
*****************************************/

bool Table::removeAttribute(int index){
   if(index >= attributeList.size())
      return false;
   attributeList.erase(attributeList.begin() + index);
   for(int i = 0; i < tableData.size(); ++i){
      tableData[i].erase(tableData[i].begin() + index);
   }
   return true;
}

Attribute Table::getAttribute(int index){
   return attributeList[index];
}

 bool Table::removeTuple(int index){
   if(index >= tableData.size())
      return false;
   if (hasPkey) {
      map<vector<string>, char>::iterator it
         = pKeys.find(toPkey(tableData[index]));
      pKeys.erase(it);
   }
   tableData.erase(tableData.begin() + index);
   return true;
}

vector<string> Table::getTuple(int index){
   if(index >= tableData.size())
      return vector<string>(0);
   return tableData[index];
}

bool Table::printTable() {
   if (attributeList.size() == 0) return false;
   cout << "{ " << attributeList[0].name;
   for(int i =1; i < attributeList.size(); i++){
      cout << " : " << attributeList[i].name;
   }
   cout << " }\n";
   for(int i =0; i < tableData.size(); i++){
      cout << "( " << tableData[i][0];
      for(int j=1; j < attributeList.size(); j++){
         cout << ", " << tableData[i][j];
      }
      cout << " )\n";
   }
   return true;
}

bool Table::addTuple(vector<string> tuple){
   //First, check types
   int i;
   for (i=0; i<tuple.size(); ++i) {
      if (getAttribute(i).type == dataType::Integer) {
         stoi(tuple[i]);   //Exception thrown if fails
      } 
      else if (getAttribute(i).type == dataType::Double) {
         stof(tuple[i]);   //Exception thrown if fails
      }
      else if (getAttribute(i).type == dataType::Varchar) {
         int first = tuple[i].find("\"");
         int second = tuple[i].find("\"", first+1);
         if (first != -1 || second != -1) {
			  tuple[i] = tuple[i].substr(1, tuple[i].size()-2);
         }
      }
   }

   if (hasPkey) {
      map<vector<string>, char>::iterator it = pKeys.find(toPkey(tuple));
      if (it == pKeys.end()) {
         tableData.push_back(tuple);
         pKeys.insert(pair<vector<string>, char>(toPkey(tuple), 'y'));
         return true;
	  }
   } else {
      tableData.push_back(tuple);
      return true;
   }

   return false;
}

bool Table::updateTuple(int index, string oldVal, string newVal) {
   //First, check types
   int ind = getAttributeIndex(oldVal);
   if (getAttribute(ind).type == dataType::Integer) {
      stoi(newVal);   //Exception thrown if fails
   } 
   else if (getAttribute(ind).type == dataType::Double) {
      stof(newVal);   //Exception thrown if fails
   }

   if(index >= tableData.size())
      return false;
   tableData[index][this->getAttributeIndex(oldVal)] = newVal;
   return true;
}

int Table::getAttributeIndex(string attributeName){
   for(int i =0; i < attributeList.size(); ++i){
      if(attributeList[i].name == attributeName){
         return i;
      }
   }
   return -1;
}

bool Table::addAttribute(Attribute attr){
   bool exists = false;
   for (int i=0; i<attributeList.size(); ++i) {
      if (attributeList[i].name.compare(attr.name) == 0)
         exists = true;
   }
   if (!exists){
      attributeList.push_back(attr);
      if (attr.key) {
         hasPkey = true;
      }
      return true;
   }
   return false;
}

Table::Table(string tableName, vector<Attribute> attributes){
   name = tableName;
   for (int i=0; i<attributes.size(); ++i) {
      this->addAttribute(attributes[i]);
   }
}

int Table::getTupleIndex(vector<string> tupleToFind){
   for(int i =0; i < tableData.size(); ++i){
      if(isSame(tupleToFind, tableData[i])){
         return i;
      }
   }
   return -1;
}

vector<string> Table::toPkey(vector<string> dataVec) {
   vector<string> toReturn;
   if (hasPkey) {
      for (int i=0; i<attributeList.size(); ++i) {
         if (attributeList[i].key) {
            toReturn.push_back(dataVec[i]);
         } else {
            toReturn.push_back("");
         }
      }
   }

   return toReturn;
}
