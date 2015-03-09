all: DBengine.o Parser.o
ar -cvq DBMS.a *.o
DBengine.o: DBengine.cpp DBengine.h Attributes.h APIdbms.h
g++ -g -c DBengine.cpp
Parser.o: Parser.cpp Parser.h Attributes.h
g++ -g -c Parser.cpp
clean:
rm *.o *.a