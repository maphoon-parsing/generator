
# Automatically generated Makefile
# Makefile-generator programmed by Hans de Nivelle, 2002


Flags = -Wreturn-type -pedantic -pedantic-errors -Wundef -std=c++17
CPP = g++


maphoon : Makefile   maphoon.o filereader.o tokenizing.o ourownparser.o symbol.o errors.o cpp.o rule.o grammar.o itemsets.o parsetable.o symbolcodebuilder.o parsercodebuilder.o tablecodebuilder.o 
	$(CPP) $(Flags) -o maphoon   maphoon.o filereader.o tokenizing.o ourownparser.o symbol.o errors.o cpp.o rule.o grammar.o itemsets.o parsetable.o symbolcodebuilder.o parsercodebuilder.o tablecodebuilder.o 

maphoon.o : Makefile   maphoon.cpp   cpp.h symbol.h parsercodebuilder.h symbolcodebuilder.h errors.h itemsets.h symbolset.h rule.h grammar.h indentation.h parsetable.h ourownparser.h filereader.h tokenizing.h 
	$(CPP) -c $(Flags) maphoon.cpp -o  maphoon.o


filereader.o : Makefile   filereader.cpp   filereader.h 
	$(CPP) -c $(Flags) filereader.cpp -o  filereader.o


tokenizing.o : Makefile   tokenizing.cpp   tokenizing.h indentation.h filereader.h 
	$(CPP) -c $(Flags) tokenizing.cpp -o  tokenizing.o


ourownparser.o : Makefile   ourownparser.cpp   errors.h itemsets.h symbolset.h symbol.h rule.h grammar.h ourownparser.h filereader.h tokenizing.h indentation.h symbolcodebuilder.h cpp.h parsercodebuilder.h parsetable.h 
	$(CPP) -c $(Flags) ourownparser.cpp -o  ourownparser.o


symbol.o : Makefile   symbol.cpp   cpp.h symbol.h 
	$(CPP) -c $(Flags) symbol.cpp -o  symbol.o


errors.o : Makefile   errors.cpp   errors.h itemsets.h symbolset.h symbol.h rule.h grammar.h 
	$(CPP) -c $(Flags) errors.cpp -o  errors.o


cpp.o : Makefile   cpp.cpp   cpp.h symbol.h 
	$(CPP) -c $(Flags) cpp.cpp -o  cpp.o


rule.o : Makefile   rule.cpp   rule.h symbol.h 
	$(CPP) -c $(Flags) rule.cpp -o  rule.o


grammar.o : Makefile   grammar.cpp   grammar.h rule.h symbol.h symbolset.h 
	$(CPP) -c $(Flags) grammar.cpp -o  grammar.o


itemsets.o : Makefile   itemsets.cpp   itemsets.h symbolset.h symbol.h rule.h grammar.h 
	$(CPP) -c $(Flags) itemsets.cpp -o  itemsets.o


parsetable.o : Makefile   parsetable.cpp   parsetable.h itemsets.h symbolset.h symbol.h rule.h grammar.h 
	$(CPP) -c $(Flags) parsetable.cpp -o  parsetable.o


symbolcodebuilder.o : Makefile   symbolcodebuilder.cpp   indentation.h cpp.h symbol.h symbolcodebuilder.h errors.h itemsets.h symbolset.h rule.h grammar.h 
	$(CPP) -c $(Flags) symbolcodebuilder.cpp -o  symbolcodebuilder.o


parsercodebuilder.o : Makefile   parsercodebuilder.cpp   tablecodebuilder.h symbol.h symbolset.h parsetable.h itemsets.h rule.h grammar.h cpp.h errors.h parsercodebuilder.h symbolcodebuilder.h indentation.h 
	$(CPP) -c $(Flags) parsercodebuilder.cpp -o  parsercodebuilder.o


tablecodebuilder.o : Makefile   tablecodebuilder.cpp   tablecodebuilder.h symbol.h symbolset.h parsetable.h itemsets.h rule.h grammar.h cpp.h 
	$(CPP) -c $(Flags) tablecodebuilder.cpp -o  tablecodebuilder.o


