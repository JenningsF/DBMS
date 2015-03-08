all: APIdbms.o DBengine.o Parser.o
	ar -cvq DBMS.a *.o
DBengine.o: DBengine.cpp DBengine.h Attributes.h
	g++ -g -c DBengine.cpp
Parser.o: Parser.cpp Parser.h Attributes.h
	g++ -g -c Parser.cpp
APIdbms.o: APIdbms.cpp APIdbms.h DBengine.h Parser.h
	g++ -g -c main.cpp
clean:
	rm *.o *.a