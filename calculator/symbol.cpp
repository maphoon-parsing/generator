
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


// This code was produced by Maphoon 2021.
// Code for struct symbol:

#include "symbol.h"


#line 83 "ambiguous.m"
 

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

}

const char* getcstring( symboltype tp )
{
   switch( tp )
   {
   case sym__recover_:
      return "_recover_";
   case sym_EMPTY:
      return "EMPTY";
   case sym_WHITESPACE:
      return "WHITESPACE";
   case sym_Show:
      return "Show";
   case sym_Session:
      return "Session";
   case sym_Arguments:
      return "Arguments";
   case sym_Quit:
      return "Quit";
   case sym_Command:
      return "Command";
   case sym_F:
      return "F";
   case sym_H:
      return "H";
   case sym_G:
      return "G";
   case sym_RPAR:
      return "RPAR";
   case sym_LPAR:
      return "LPAR";
   case sym_EOF:
      return "EOF";
   case sym_SCANERROR:
      return "SCANERROR";
   case sym_COMMENT:
      return "COMMENT";
   case sym_SEMICOLON:
      return "SEMICOLON";
   case sym_IDENTIFIER:
      return "IDENTIFIER";
   case sym_BECOMES:
      return "BECOMES";
   case sym_COMMA:
      return "COMMA";
   case sym_DOUBLE:
      return "DOUBLE";
   case sym_PLUS:
      return "PLUS";
   case sym_TIMES:
      return "TIMES";
   case sym_MINUS:
      return "MINUS";
   case sym_DIVIDES:
      return "DIVIDES";
   case sym_E:
      return "E";
   case sym_MODULO:
      return "MODULO";
   case sym_FACTORIAL:
      return "FACTORIAL";
   }
   return "(unknown symbol type)";
      // unreachable, but makes the compiler shut up
}

// If you see a big error message originating from
// symbol::print( ), then the most likely reason is that
// one of the possible attribute types or the info type has 
// no definition for operator << .
// You can either remove it from print( ), or 
// define operator << (if necessary as dummy). 

void symbol::print( std::ostream& out ) const
{
   out << "sym_" << type << "(";

   if( info. has_value( ))
      out << info. value( );

   if( std::holds_alternative< std::monostate > ( attr ))
      { out << ")"; return; }

   if( info. has_value( ))
      out << ",";

   if( std::holds_alternative< std::string > ( attr ))
      { out << std::get<std::string> ( attr ) << ")"; return; }
   if( std::holds_alternative< double > ( attr ))
      { out << std::get<double> ( attr ) << ")"; return; }
   if( std::holds_alternative< std::vector<double> > ( attr ))
      { out << std::get<std::vector<double>> ( attr ) << ")"; return; }

   throw std::runtime_error( "should be unreachable" );
}

