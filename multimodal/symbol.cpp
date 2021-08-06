
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


const char* getcstring( symboltype tp )
{
   switch( tp )
   {
   case sym_NUMBER:
      return "NUMBER";
   case sym__recover_:
      return "_recover_";
   case sym_WHITESPACE:
      return "WHITESPACE";
   case sym_BECOMES:
      return "BECOMES";
   case sym_EOF:
      return "EOF";
   case sym_RETURN:
      return "RETURN";
   case sym_COMMENT:
      return "COMMENT";
   case sym_ERROR:
      return "ERROR";
   case sym_DIAEND:
      return "DIAEND";
   case sym_Session:
      return "Session";
   case sym_DIASTART:
      return "DIASTART";
   case sym_OR:
      return "OR";
   case sym_SEMICOLON:
      return "SEMICOLON";
   case sym_Var:
      return "Var";
   case sym_EQUIV:
      return "EQUIV";
   case sym_BOX:
      return "BOX";
   case sym_Formula:
      return "Formula";
   case sym_DIA:
      return "DIA";
   case sym_Command:
      return "Command";
   case sym_CONST:
      return "CONST";
   case sym_LEFTPAR:
      return "LEFTPAR";
   case sym_Index:
      return "Index";
   case sym_NOT:
      return "NOT";
   case sym_ID:
      return "ID";
   case sym_RIGHTPAR:
      return "RIGHTPAR";
   case sym_IMP:
      return "IMP";
   case sym_BOXSTART:
      return "BOXSTART";
   case sym_AND:
      return "AND";
   case sym_BOXEND:
      return "BOXEND";
   case sym_NNF:
      return "NNF";
   case sym_METAVAR:
      return "METAVAR";
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

   if( std::holds_alternative< std::monostate > ( attr ))
      { out << ")"; return; }

   if( std::holds_alternative< agent > ( attr ))
      { out << std::get<agent> ( attr ) << ")"; return; }
   if( std::holds_alternative< bool > ( attr ))
      { out << std::get<bool> ( attr ) << ")"; return; }
   if( std::holds_alternative< form > ( attr ))
      { out << std::get<form> ( attr ) << ")"; return; }
   if( std::holds_alternative< std::string > ( attr ))
      { out << std::get<std::string> ( attr ) << ")"; return; }
   if( std::holds_alternative< unsigned int > ( attr ))
      { out << std::get<unsigned int> ( attr ) << ")"; return; }

   throw std::runtime_error( "should be unreachable" );
}

