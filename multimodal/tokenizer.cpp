
//  Copyright <2021> <Hans de Nivelle>
//
//  This software is released with the 3-clause BSD license, which 
//  is given below: 
// 
//  Redistribution and use in source and binary forms, with or without 
//  modification, are permitted provided that the following conditions are met:

//  Redistributions of source code must retain the above copyright notice, 
//  this list of conditions and the following disclaimer:

//  Redistributions in binary form must reproduce the above copyright notice, 
//  this list of conditions and the following disclaimer in the documentation 
//  and/or other materials provided with the distribution.
//  Neither the name of the copyright holder nor the names of its contributors 
//  may be used to endorse or promote products derived from this software 
//  without specific prior written permission.

//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
//  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
//  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
//  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include "tokenizer.h"

#include "trivial.h"

#if 0
lexing::classifier< char, symboltype > 
tokenizer::getclassifier( )
{
   lexing::classifier< char, symboltype > cls( sym_ERROR ); 

   cls. insert( lexing::just( ';' ), sym_SEMICOLON );
  
   auto idfirst = lexing::range('a','z') | lexing::range('A','Z');
   auto idnext = lexing::range('a','z') | lexing::range('A','Z') | 
                    lexing::just('_') | lexing::range('0','9');

   cls. insert( idfirst * idnext. star( ), sym_ID );

   cls. insert( lexing::just( '$' ) * idfirst * idnext. star( ), sym_METAVAR );

   auto number = lexing::just('0') |
                ( lexing::range('1','9') *
                        lexing::range('0','9'). star());
  
   cls. insert( number, sym_NUMBER ); 
   cls. insert( lexing::word( "TRUE" ) |
                lexing::word( "true" ) |
                lexing::word( "FALSE" ) |
                lexing::word( "false" ), sym_CONST );

   cls. insert( lexing::word( "NNF" ), sym_NNF );
   cls. insert( lexing::word( "RETURN" ), sym_RETURN );

   cls. insert( lexing::just( '~' ), sym_NOT );
   cls. insert( lexing::just( '&' ), sym_AND );
   cls. insert( lexing::just( '|' ), sym_OR );
   cls. insert( lexing::word( "->" ), sym_IMP );
   cls. insert( lexing::word( "<->" ), sym_EQUIV ); 
   cls. insert( lexing::word( "[]" ), sym_BOX );
   cls. insert( lexing::word( "<>" ), sym_DIA );

   cls. insert( lexing::just( '(' ), sym_LEFTPAR );
   cls. insert( lexing::just( ')' ), sym_RIGHTPAR );
   cls. insert( lexing::just( '[' ), sym_BOXSTART );
   cls. insert( lexing::just( ']' ), sym_BOXEND );
   cls. insert( lexing::just( '<' ), sym_DIASTART );
   cls. insert( lexing::just( '>' ), sym_DIAEND );

   cls. insert( lexing::just( '=' ), sym_BECOMES );

   cls. insert( lexing::just( '#' ), sym_EOF );

   // Whitespace, and the likes of them: 

   cls. insert( ( lexing::just(' ') | lexing::just('\n') | 
                  lexing::just('\t') ). plus( ), sym_WHITESPACE );

   cls. insert( lexing::word( "//" ) *
                ( lexing::every<char>( ). without( '\n' )). star( ) * 
                lexing::just( '\n' ), sym_COMMENT );

   cls. insert( lexing::word( "/*" ) *
      ( lexing::every<char>( ). without( '*' ) |
        ( lexing::just('*'). plus( ) * 
          lexing::every<char> ( ). without( '/' ). without( '*' )). star( )
        ). star( ) * 
      lexing::just( '*' ). plus( ) * lexing::just( '/' ), sym_COMMENT );

   cls = make_deterministic( cls );
   std::cout << "automaton has " << cls. nrstates( ) << " states\n";
   cls = minimize( cls );
   std::cout << "after minimization " << cls. nrstates( ) << "\n";
   auto nrstates = cls. nrstates( );
   cls = minimize( make_deterministic( cls ));
   if( cls. nrstates( ) < nrstates )
   {
      std::cout << "it went further down to " << cls. nrstates( ) << "\n";
      std::cout << "that is a bug\n";
      throw std::runtime_error( "bug" ); 
   }
   std::cout << "automaton " << cls << "\n";

   std::ofstream file( "code.h" );
   lexing::printcode<char, symboltype> (
     "char", "symboltype", "",
      cls, file,
      []( std::ostream& out, char c ) { out << (int)c; },
      []( std::ostream& out, const symboltype t ) { out << "sym_" << t; } );
   file. close( );

   return cls;
}
#endif

#include "code.h"

symbol tokenizer::get( ) 
{
   // static auto cls = getclassifier( ); 
   static auto cls = lexing::trivial( );
 
restart:
   // Restart after whitespace, or comment

   if( !read. has(1) )
      return symbol( sym_EOF );

   auto p = readandclassify( cls, read );
   // std::cout << "p = " << p. first << "/" << p. second << "\n";   

   // I think it has to be done like this.
   // Or one creates a garbage token with lowest priority.
   // Explictly creating an error token of size 1 is wrong.

   if( p. second == 0 )
   {
      read. commit(1);
      return symbol( sym_ERROR );
   }

   // If whitespace or blabla, we restart:

   if( p. first == sym_WHITESPACE || p. first == sym_COMMENT )
   {
      read. commit( p. second );
      goto restart;
   }

   if( p. first == sym_NUMBER )
   {
      unsigned int val = std::stoul( std::string( read. view( p. second )));
      read. commit( p. second );
      return symbol( sym_NUMBER, val );
   }

   if( p. first == sym_ID || p. first == sym_METAVAR )  
   {
      std::string name = std::string( read. view( p. second ));
      read. commit( p. second );
      return symbol( p. first, name );
   }

   if( p. first == sym_CONST )
   {
      bool b = false;
      if( read. peek(0) == 't' || read. peek(0) == 'T' )
         b = true;
      read. commit( p. second );
      return symbol( sym_CONST, b );
   }

   // All remaining symbols have no attribute:

   read. commit( p. second );
   return symbol( p. first ); 
}


void tokenizer::test( ) 
{
   symbol sym = get( ); 
   while( sym.type != sym_EOF && sym.type != sym_SEMICOLON )
   {
      std::cout << sym << "\n";
      sym = get( ); 
   }
   std::cout << "that's the end of the file\n";
}


void tokenizer::print( std::ostream& out ) const
{
   out << "tokenizer: " << read;
}


