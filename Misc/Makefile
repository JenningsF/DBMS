all: main.o DBengine.o Parser.o
	g++ main.o Parser.o DBengine.o -o DBMS
DBengine.o: DBengine.cpp DBengine.h Attributes.h
	g++ -g -c DBengine.cpp
Parser.o: Parser.cpp Parser.h Attributes.h
	g++ -g -c Parser.cpp
main.o: main.cpp DBengine.h Parser.h
	g++ -g -c main.cpp
clean:
	rm *.o DBMS