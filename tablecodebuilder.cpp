
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


// Written by Hans de Nivelle, March 2021.

#include "tablecodebuilder.h"

size_t 
tablecodebuilder::statesymbol::hash::operator( ) ( 
                       const statesymbol& ss ) const
{
   symbol::hash h; 
   return ss. state + 11 * h( ss. sym ); 
}

bool
tablecodebuilder::statesymbol::equal_to::operator( ) ( 
                       const statesymbol& ss1,
                       const statesymbol& ss2 ) const
{
   return ( ss1. state == ss2. state ) && ( ss1. sym == ss2. sym );
}


void
tablecodebuilder::printinspace( const cpp::nspace& space,
                                const char* c,
                                std::ostream& out ) const
{
   space. printasprefix( out );
   out << c;
}

void 
tablecodebuilder::print( const cpp::nspace& space, symbol s, 
                         std::ostream& out ) const
{
   space. printasprefix( out );
   out << "sym_" << s;
}

void tablecodebuilder::print( const cpp::nspace& space, statesymbol ss,
                              std::ostream& out ) const
{
   out << "{ " << ss. state << ", ";
   print( space, ss. sym, out ); 
   out << " }"; 
}

void 
tablecodebuilder::printascode( std::ostream& out, 
                               const cpp::nspace& parserspace,
                               const cpp::nspace& symbolspace ) const
{
   // The start symbols:

   out << "std::unordered_map< ";
   printinspace( symbolspace, "symboltype", out );
   out << ", std::pair< size_t, std::unordered_set< ";
   printinspace( symbolspace, "symboltype >>> ", out );
   printinspace( parserspace, "parser::startsymbols =\n", out );
   out << "{\n";

   size_t cnt = startsymbols. size( ); 
   for( const auto& p : startsymbols ) 
   {
      out << "   { ";
      print( symbolspace, p. first, out );
      out << ", { " << p. second. first << ", { ";
      for( auto q = p. second. second. begin( );
                q != p. second. second. end( );
                ++ q )
      {
         if( q != p. second. second. begin( ))
            out << ", ";
         print( symbolspace, *q, out );
      }
      out << " }} }";
      if( -- cnt ) out << ",\n";
   }
   out << "\n};\n";

   out << "\n";

   // Information about reductions per state:

   out << "std::vector< std::pair< char, size_t >> ";
   printinspace( parserspace, "parser::reductioninfo =\n", out );
   out << "{\n   ";

   for( size_t i = 0; i != reductioninfo. size( ); ++ i )
   {
      if( i != 0 ) 
      {
         if( i % 8 == 0 )
            out << ",\n   ";
         else
            out << ", ";
      }
      out << "{ " << "'" << reductioninfo[i]. first << "', ";
      out << reductioninfo[i]. second << " }";
   }
   out << "\n};\n\n";

   printinspace( parserspace, "parser::statesymbol::map< size_t > ", out );
   printinspace( parserspace, "parser::shifts =\n", out ); 
   out << "{\n   ";

   size_t nr = 0; 
   for( const auto& p : shifts ) 
   {
      if( nr != 0 )
      {
         if( nr % 5 == 0 )
            out << ",\n   ";
         else
            out << ", ";
      }
      out << "{ "; print( symbolspace, p. first, out ); 
      out << ", " << p. second << " }";
      ++ nr;
   }
   out << "\n};\n\n";
   
   printinspace( parserspace, 
                 "parser::statesymbol::map< std::vector< size_t >> ", out );
   printinspace( parserspace, "parser::reductions =\n", out );   

   out << "{\n   ";
     
   nr = 0;
   for( const auto& p : reductions )
   {
      if( nr != 0 )
      {
         if( nr % 4 == 0 )
            out << ",\n   ";
         else
            out << ", ";
      }
      out << "{ "; print( symbolspace, p. first, out );
      out << ", { ";
      for( size_t i = 0; i != p. second. size( ); ++ i ) 
      {
         if(i) out << ", ";
         out << p. second[i];
      }
      out << " } }";
      ++ nr; 
   } 
   out << "\n};\n\n"; 

}

