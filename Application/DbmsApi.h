#ifndef DBMS_API_H
#define DBMS_API_H

#include <string>
#include "Table.h"

enum DBMSErrorCode {
   goodParse = 0,
   badParse = 1,
   noAttribute = 2,
   incompatibleTypes = 3,
   badType = 4,
   noTable = 5,
   ioFailure = 6,
   ioNoFile = 7,
   incompatibleTables = 8
};

int getLastDBMSError();
string getLastDBMSErrorDetail();

bool DBMSParse(string dbmsStr);

Table GetRelation(string relation);

#endif