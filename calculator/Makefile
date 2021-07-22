
# Automatically generated Makefile
# Makefile-generator programmed by Hans de Nivelle, 2002


Flags = -Wreturn-type -pedantic -pedantic-errors -Wundef -std=c++17
CPP = g++


calculator : Makefile   calculator.o symbol.o tokenizing.o parser.o filereader.o 
	$(CPP) $(Flags) -o calculator   calculator.o symbol.o tokenizing.o parser.o filereader.o 

calculator.o : Makefile   calculator.cpp   parser.h symbol.h varstore.h filereader.h tokenizing.h 
	$(CPP) -c $(Flags) calculator.cpp -o  calculator.o


symbol.o : Makefile   symbol.cpp   symbol.h varstore.h 
	$(CPP) -c $(Flags) symbol.cpp -o  symbol.o


tokenizing.o : Makefile   tokenizing.cpp   tokenizing.h filereader.h symbol.h varstore.h 
	$(CPP) -c $(Flags) tokenizing.cpp -o  tokenizing.o


parser.o : Makefile   parser.cpp   parser.h symbol.h varstore.h filereader.h tokenizing.h 
	$(CPP) -c $(Flags) parser.cpp -o  parser.o


filereader.o : Makefile   filereader.cpp   filereader.h 
	$(CPP) -c $(Flags) filereader.cpp -o  filereader.o


