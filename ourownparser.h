
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



#ifndef MAPH_OUROWNPARSER_
#define MAPH_OUROWNPARSER_  1

#include <vector>

#include "filereader.h"
#include "tokenizing.h"
#include "errors.h"
#include "symbolcodebuilder.h"
#include "parsercodebuilder.h"
#include "cpp.h"

struct ourownparser
{
   filereader read;
   std::pair< token, size_t > lookahead;

   ourownparser( filereader&& read )
      : read( std::move( read )),
        lookahead( std::pair( tok_empty, 0 ))
   { } 

   void maph_forward( ); 
      // Forward for Maphoon input. 
      // It ignores comments and whitespace. 

   void cpp_forward( );
      // Forward inside C++ code.
      // Comments and whitespace are returned like separate
      // token.

   void throwsyntaxerror( const std::string& problem );
   void throwgeneralerror( const std::string& problem );
      // throw their way out of here. 
   
   std::string_view conv2view( );

   std::vector< std::string > readcpptype( );
      // Assuming that lookahead is tok_lbrace/1,
      // read the rest of the C++ type, including
      // the closing tok_rbrace/1. 

   void readheadercode( cpp::codefragment& header ); 
      // Assuming that lookahead is already tok_lbrace/1,
      // read until and including the closing tok_rbrace/1. 

   void readstandardcode( cpp::codefragment& code );
      // As above.

   cpp::nspace readspace( );

   void startsymbol( grammar& gr ); 
   void infotype( cpp::type& tp );
   void symbols( symbolcodebuilder& bld_sym );
   void parameter( parsercodebuilder& bld_parse );
   void code_h( cpp::codefragment& code_h );
   void code_cpp( cpp::codefragment& code_cpp ); 

   void rulegroup( parsercodebuilder& bld_parse ); 
      // Group of rules that share an lhs.

   symbol::map< size_t > redsequence( );
      // Parse a line %redsequence ...
 
   void print( std::ostream& ) const;
};


inline std::ostream& 
operator << ( std::ostream& out, const ourownparser& parse )
{
   parse. print( out );
   return out;
}

#endif

