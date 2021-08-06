
# Automatically generated Makefile
# Makefile-generator programmed by Hans de Nivelle, 2002


Flags = -Wpedantic -pedantic-errors -std=c++17
CPP = g++


multimodal : Makefile   multimodal.o tokenizer.o filereader.o symbol.o form.o agent.o parser.o 
	$(CPP) $(Flags) -o multimodal   multimodal.o tokenizer.o filereader.o symbol.o form.o agent.o parser.o 

multimodal.o : Makefile   multimodal.cpp   varstore.h form.h agent.h indentation.h shared_ptr.h parser.h symbol.h tokenizer.h filereader.h 
	$(CPP) -c $(Flags) multimodal.cpp -o  multimodal.o


tokenizer.o : Makefile   tokenizer.cpp   code.h tokenizer.h filereader.h symbol.h form.h agent.h indentation.h shared_ptr.h 
	$(CPP) -c $(Flags) tokenizer.cpp -o  tokenizer.o


filereader.o : Makefile   filereader.cpp   filereader.h 
	$(CPP) -c $(Flags) filereader.cpp -o  filereader.o


symbol.o : Makefile   symbol.cpp   symbol.h form.h agent.h indentation.h shared_ptr.h 
	$(CPP) -c $(Flags) symbol.cpp -o  symbol.o


form.o : Makefile   form.cpp   form.h agent.h indentation.h shared_ptr.h 
	$(CPP) -c $(Flags) form.cpp -o  form.o


agent.o : Makefile   agent.cpp   agent.h 
	$(CPP) -c $(Flags) agent.cpp -o  agent.o


parser.o : Makefile   parser.cpp   parser.h symbol.h form.h agent.h indentation.h shared_ptr.h tokenizer.h filereader.h varstore.h 
	$(CPP) -c $(Flags) parser.cpp -o  parser.o


