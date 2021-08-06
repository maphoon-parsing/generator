
// box Alice p and box Bob q then box Alice (q and p) 
// box Alice p or box q 

%startsymbol Session EOF 

%symbol Session

%symbol ERROR 
%symbol COMMENT WHITESPACE   
%symbol SEMICOLON EOF 
%symbol { std::string } ID 
%symbol { std::string } Var 
%symbol { unsigned int } NUMBER 
%symbol { bool } CONST 
%symbol NOT 
%symbol IMP AND OR EQUIV 
%symbol BOX DIA
%symbol LEFTPAR RIGHTPAR
%symbol BOXSTART BOXEND
%symbol DIASTART DIAEND

%symbol BECOMES RETURN NNF 
%symbol { std::string } METAVAR

%symbol { form } Formula 
%symbol { form } Command 
%symbol { agent } Index

// %nodefaults
   // Turning this on makes the parser unsuitable for interactive use.

// %usererror    
   // Means that the user prefer to define their own error. 

%symbolcode_h{ #include "form.h" }
%parsercode_h{ #include "tokenizer.h" }
%parsercode_h{ #include "varstore.h" }

%parsercode_cpp
{
   namespace
   {

      // We are deciding about reducing t1.
      // 1 : we reduce
      // 0 : we don't reduce
      // -1 : we do not even shift.

      short int decide( symboltype t1, symboltype t2 ) 
      {
         // std::cout << "Deciding between: " << t1 << " " << t2 << "\n";

         if( t1 == sym_NOT || t1 == sym_DIA || t1 == sym_BOX || t1 == sym_AND )
            return 1;

         if( t1 == sym_OR )
         {
            if( t2 == sym_AND ) 
               return 0;
            return 1;
         }

         if( t1 == sym_IMP )
         {
            if( t2 == sym_AND || t2 == sym_OR || t2 == sym_IMP )
               return 0;
            return 1;
         }

         if( t1 == sym_EQUIV )
         {
            if( t2 == sym_AND || t2 == sym_OR || t2 == sym_IMP )
               return 0; 
            if( t2 == sym_EQUIV )
               return -1;
            return 1; 
         }

         throw std::runtime_error( "should be unreachable" ); 
      }
   }
}


%source { tok. get( ); }
%parameter { tokenizer } tok 
%parameter { varstore<form> } vs

%keepclosures   
   // If you put it on, the states will be shown closed. 
   // It doesn't affect the parser constructed. 

%rules

Session => Session Command | 
           ; 

Command => Formula: f SEMICOLON
   { std::cout << "    formula: "; f. printprefix( std::cout ); 
     std::cout << "\n"; return f; }

| RETURN Formula: f SEMICOLON
   { std::cout << "returning ";
     f. printprefix( std::cout ); 
     timetosaygoodbye = true; return f; }

| METAVAR:var BECOMES Formula : f SEMICOLON
    { vs. assign( var, f ); return f; }

| _recover_ SEMICOLON
    { return form( "ERROR" ); }  

| RIGHTPAR _recover_ SEMICOLON 
   { return form( "ERROR" ); }

| BOXEND _recover_ SEMICOLON 
   { return form( "ERROR" ); }
   
;

Formula => Var:v { return v; } 

|  CONST:b { return form( b ? op_true : op_false ); } 

|  NOT Formula:f

%requires
   { return decide( sym_NOT, lookahead. value( ). type ); }
%reduces
   { return form( op_not, f ); } 

|  Formula:f1 AND Formula:f2   

%requires
   { return decide( sym_AND, lookahead. value( ). type ); }
%reduces 
   { return form( op_and, f1, f2 ); }  

|  Formula:f1 OR Formula:f2   

%requires
   { return decide( sym_OR, lookahead. value( ). type ); } 
%reduces 
   { return form( op_or, f1, f2 ); }

|  Formula:f1 IMP Formula:f2  

%requires
   { return decide( sym_IMP, lookahead. value( ). type ); } 
%reduces
   { return form( op_imp, f1, f2 ); }

|  Formula:f1 EQUIV Formula:f2

%requires
   { return decide( sym_EQUIV, lookahead. value( ). type ); } 
%reduces 
   { return form( op_equiv, f1, f2 ); }

|  BOX Index:a Formula:f 

%requires
   { return decide( sym_BOX, lookahead. value( ). type ); }
%reduces
   { return form( op_box, a, f ); }

|  BOXSTART Index:a BOXEND Formula:f 

%requires
   { return decide( sym_BOX, lookahead. value( ). type ); }
%reduces
   { return form( op_box, a, f ); } 

|  DIA Index:a Formula:f

%requires
   { return decide( sym_DIA, lookahead. value( ). type ); }
%reduces
   { return form( op_dia, a, f ); }

|  DIASTART Index:a DIAEND Formula:f 

%requires
   { return decide( sym_DIA, lookahead. value( ). type ); }
%reduces
   { return form( op_dia, a, f ); }

|  BOX Formula:f 

%requires
   { return decide( sym_DIA, lookahead. value( ). type ); }
%reduces
   { return form( op_box, "#1", f ); } 

|  DIA Formula:f 

%requires
   { return decide( sym_BOX, lookahead. value( ). type ); }
%reduces
   { return form( op_dia, "#1", f ); }

|  LEFTPAR Formula:f RIGHTPAR  %reduces { return f; } 

|  LEFTPAR _recover_ RIGHTPAR 
      { return form( op_dia, agent( "ERROR" ), op_false ); }

|  METAVAR : var
   
   {
      if( vs. contains( var ))
         return *vs. lookup( var ); 
      else
         return form( op_dia, agent( "NOT FOUND" ), op_false ); 
   }

|  NNF LEFTPAR Formula:f RIGHTPAR
   { return f. nnf(1); } 

|  NNF LEFTPAR _recover_ RIGHTPAR 
   { return form( op_dia, agent( "wrong NNF" ), op_false ); }

;

Var => ID:v { return v; } ; 

Index => ID:v { return v; } 
       | NUMBER:n { return std::string( "#" ) + std::to_string(n); }
       ;

