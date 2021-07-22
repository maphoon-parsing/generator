
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


// Written by Hans de Nivelle, Spring 2021.

#ifndef MAPH_TOKENIZING
#define MAPH_TOKENIZING  1

#include <iostream> 
#include <map>

#include "indentation.h"
#include "filereader.h"

enum token { tok_rewrite,         // =>
             tok_bar,             // |
             tok_colon,           // :
             tok_semicolon,       // ;
 
             tok_symbol,          // %symbol 
             tok_startsymbol,     // %startsymbol 
             tok_infotype,        // %infotype
             tok_parameter,       // %parameter
             tok_symbolspace,     // %symbolspace
             tok_parserspace,     // %parserspace 
             tok_symbolcode_h,    // %symbolcode_h
             tok_symbolcode_cpp,  // %symbolcode_cpp
             tok_parsercode_h,    // %parsercode_h
             tok_parsercode_cpp,  // %parsercode_cpp
             tok_source,          // %source
             tok_nodefaults,      // %nodefaults
             tok_keepclosures,    // %keepclosures
             tok_selfcheck,       // %selfcheck 
             tok_redsequence,     // %redsequence
             tok_rules,           // %rules
             tok_requires,        // %requires
             tok_reduces,         // %reduces
             tok_usererror,       // %usererror 

             tok_recover,         // _recover_ 

             tok_lpar,
             tok_rpar,
             tok_lbrace, 
             tok_rbrace,
             tok_lbracket,
             tok_rbracket,

             tok_lt,              // < ,
             tok_gt,              // > , 
             tok_separator,       // :: 
             tok_star,            // * 
             tok_ampersand,
             tok_comma, 
             tok_dot, 

             tok_identifier,
             tok_unsigned, 
             tok_double,     
             tok_wrongdouble, 
             tok_string,
             tok_wrongstring,   // more informative, than just 'error'

             tok_cpp_include,   // The C++ preprocessor #include 
             tok_cpp_define,    // The C++ preprocessor #define 
             tok_cpp_operator,  // All remaining operators that have no 
                                // special meaning for us. 

             tok_error,
             tok_comment, 
             tok_wrongcomment,
             tok_space,
             tok_empty,        // The empty token has zero length. 
             tok_eof };


const char* getcstring( token tok );

inline std::ostream& operator << ( std::ostream& out, token tok )
{
   out << getcstring( tok ); return out;
}

inline 
std::ostream& operator << ( std::ostream& out, 
                            std::pair< token, size_t > t )
{
   return out << t. first << "/" << t. second; 
}

// Earlier version of this class were polymorphic, but 
// here we don't need that.

struct trie
{
   token tok; 
   std::map< char, trie > rest; 

   // By default, we put tok_empty at the empty string: 

   trie( token tok = tok_empty ) 
      : tok( tok ) 
   { } 

   // If we are forced to create a subtrie, we mark it with
   // tok_error. 

   void insert( const char* c, token tok ); 
      // c is a C-string. 

   std::pair< token, size_t > readfrom( filereader& ) const; 
      // Read input until we get stuck, and return the token at that point. 

   void print( std::ostream& out, indentation ind ) const; 
};

inline std::ostream& operator << ( std::ostream& out, const trie & tr )
{
   tr. print( out, indentation( ));
   return out;
}

namespace tokenize
{
   double conv2double( filereader& read, size_t );
   size_t conv2unsigned( filereader& read, size_t );
   std::string_view conv2view( filereader& read, size_t );
      // Always from the initial segment.

   std::pair< token, size_t > read_whitespace( filereader& read );
   std::pair< token, size_t > read_unsigned( filereader& read );
   std::pair< token, size_t > read_double( filereader& read ); 
   std::pair< token, size_t > read_identifier( filereader& read );
   std::pair< token, size_t > read_comment( filereader& read );
   std::pair< token, size_t > read_string( filereader& read );
      // These functions return a pair with zero length when they
      // could not recognize anything. In that case, one should simply try
      // something else. In theory, one should always take the longest fit,
      // but in nearly all cases, the first nonzero fit is also the longest. 
 
   std::pair< token, size_t > read( const trie& tr, filereader& read );

   std::pair< token, size_t > maph( filereader& read );

   std::pair< token, size_t > cpp( filereader& read );
      // I think that inside C++ code, the tokens are always the 
      // same. It doesn't matter if it is a type definition, an include,
      // or action code. 
      // cpp does not ignore whitespace or comments, but returns them.
      // We tokenize C++ code, so that we can normalize types (somewhat),
      // and identify identifiers that are subject to replacement.

}

#endif 

