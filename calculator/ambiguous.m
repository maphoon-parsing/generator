
// A sample Maphoon file. 

%startsymbol Session EOF 
%startsymbol E SEMICOLON
   // Defines a start symbol, together with its follow set. 

// Fields that all symbols have.
// Don't put heavy things here. It is intended for light things like
// line numbers, etc. 

%redsequence Quit Show H
 
%selfcheck
// %nodefaults 

%infotype{ location } 
   // Type of source information. It is optionally present in every token. 

// Symbols and their attributes:

%symbol{       	     }             EOF 
%symbol{ std::string }             SCANERROR IDENTIFIER 
%symbol                            SEMICOLON BECOMES COMMA 
%symbol{ double }                  DOUBLE
%symbol                            PLUS TIMES MINUS DIVIDES MODULO
%symbol                            FACTORIAL
%symbol                            LPAR RPAR 
%symbol{ double }                  E F G H 
%symbol{ std::vector<double> }     Arguments  
%symbol                            Session Command 
%symbol                            Quit Show

   // No attribute type means void. 
%symbol{}                          COMMENT WHITESPACE EMPTY
   // These symbols are used internally in the tokenizer.
   // One still has to declare them. 

%parameter{ varstore<double> }             memory 
%parameter{ std::vector< std::string > }   errorlog 
%parameter{ filereader }                   read 
   // Declares additional parameters to the parser.
   // They are reference parameters to the parser, 
   // and they can be used in action code. 
   // In this case, a reference variable varstore,
   // and a reference variable errorlog. 
   // There is no point in allowing local variables
   // in the parser.
   // If you want to read input from a file, or from
   // somewhere, you have to include it here.

// This goes into the beginning of symbol.h :

%symbolcode_h{ #include "varstore.h" }
%symbolcode_h{ #include <math.h> }
%symbolcode_h{ #include <vector> } 
%symbolcode_h{
struct location 
{
   const std::string* filename; 
   long unsigned int line;
   long unsigned int column;

   location( const std::string* filename,
             long unsigned int line,
             long unsigned int column )
      : filename( filename ),
        line( line ),
        column( column )
   { }

};

   std::ostream& operator << ( std::ostream& , const location& ); 
}

// This code goes into the beginning of symbol.cpp. 
// I recommend that you always put it in anonymous namespace.
// If you don't want to do that, then at least make sure to declare
// everything in symbolcode_h.
// It is intended for definitions of print functions of attributes.

%symbolcode_cpp { 

   std::ostream& 
   operator << ( std::ostream& out, const location& loc )
   {
      out << "from file " << * ( loc. filename )  << " at position " <<
            loc. line << "/" << loc. column; 
      return out;
   }

namespace {
   std::ostream& operator << ( std::ostream& out,
                               const std::vector< double > & vect )
   {
      out << "( ";
      for( size_t i = 0; i != vect. size( ); ++ i )
      {
         if( i != 0 ) out << ", ";
         out << vect[i];
      }
      out << ")";
      return out;
   }

}}

%parsercode_h { #include "varstore.h" }
%parsercode_h { #include "filereader.h" }
%parsercode_h { #include "tokenizing.h" }

%parsercode_cpp {

namespace {
   double fact( unsigned int n )
   {
      double res = 1.0;
      while( n )
      {
         res *= n;
         -- n;
      }
      return res;
   }

   void printerrors( const std::vector< std::string > & errors, 
                     std::ostream& out )
   {
      std::cout << "Errors:\n";
      for( const auto& err : errors )
         out << "   " << err << "\n";
   }

}}
   
// Namespaces of symbol, tokenizer and parser.
// One should probably put them in the same namespace.

%symbolspace 
%parserspace 

%source{ tokenizing::readsymbol( read ); } 
   // Source from where the symbols come. 
   // It must compile in a situation like s = readsymbol( ); 

%rules

Session => Session Command 
|
;

Command => E:e SEMICOLON
{
   if( errorlog. size( ))
   {
      printerrors( errorlog, std::cout ); 
      errorlog. clear( ); 
   }
   else
      std::cout << "Result is " << e << "\n"; 
}


| IDENTIFIER:id BECOMES E:e SEMICOLON 

{
   if( errorlog. empty( )) 
   {
      std::cout << "assigning: " << id << " := " << e << "\n";
      memory. assign( id, e ); 
   }
   else
   {
      printerrors( errorlog, std::cout ); 
      errorlog. clear( ); 
   }
}
| Quit SEMICOLON 
{
   std::cout << "quitting\n"; timetosaygoodbye = true;
}
| Show SEMICOLON
{
   std::cout << memory << "\n";
}
|  _recover_ SEMICOLON
{
   std::cout << "recovered from syntax error\n\n";
}
;

E  => E:e PLUS F:f   { return e + f; }
    | E:e MINUS F:f  { return e - f; }
    | F : f          { return f; }
    ;

F  => F:f TIMES G:g { return f * g; }
| F:f DIVIDES G:g 

{
   if( g == 0.0 ) 
   {
      errorlog. push_back( "division by zero" ); 
         g = 1.0;   // invent a value. 
   }
   return f / g;  
}

| F:f MODULO G:g    // here you can put a comment 

{
   if( g == 0.0 )
   {
      errorlog. push_back( "modulo by zero" );
      g = 1.0;
   }
    
   return f - g * floor( f / g ); 
}

| G : g   /* here can also be comment */ { return g; }
;

G => MINUS G : g    { return -g; }
  | PLUS G : g      { return g; } 
  | H : h           { return h; }
  ;

H  => H:h FACTORIAL
{
   unsigned int f = static_cast< unsigned int >
         ( floor( h + 0.0001 ));
   return fact(f);  
}
 
| LPAR E:e RPAR  { return e; }
| IDENTIFIER : id 
{
   if( memory. contains(id))
      return *memory. lookup(id);
   else
   {
      errorlog. push_back( std::string( "variable " ) + id + 
                           " is undefined " );
      return 0.0;  
   }
}

| DOUBLE : d   { return d; } 

| IDENTIFIER:id LPAR Arguments:args RPAR 
{ 
   if( id == "sin" && args. size( ) == 1 ) 
      return sin( args[0] ); 

   if( id == "cos" && args. size( ) == 1 )
      return cos( args[0] );

   if( id == "sqrt" && args. size( ) == 1 )
      return sqrt( args[0] ); 

   if( id == "pow" && args. size( ) == 2 )
   {
      return pow( args[0], args[1] );
   }

   
   errorlog. push_back( std::string( "unrecognized function " ) + id );
   return 0.0;
}
  
;

Arguments => E:e          {  return { e };   }
| Arguments:a COMMA E:e   { a. push_back(e); return a; }
;

Quit => IDENTIFIER : id
%requires { return id == "quit"; }
;

Show => IDENTIFIER : id 
%requires { return id == "show"; }
;


