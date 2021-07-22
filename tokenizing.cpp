
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


// Written by Hans de Nivelle, February 2021.

#include "tokenizing.h"

const char* getcstring( token tok )
{
   switch( tok )
   {
   case tok_rewrite:         return "rewrite";
   case tok_bar:             return "bar";
   case tok_colon:           return "colon";
   case tok_semicolon:       return "semicolon";

   case tok_symbol:          return "%symbol";
   case tok_startsymbol:     return "%startsymbol";
   case tok_infotype:        return "%infotype";
   case tok_parameter:       return "%parameter";
   case tok_symbolspace:     return "%symbolspace";
   case tok_parserspace:     return "%parserspace";
   case tok_symbolcode_h:    return "%symbolcode_h"; 
   case tok_symbolcode_cpp:  return "%symbolcode_cpp"; 
   case tok_parsercode_h:    return "%parsercode_h";
   case tok_parsercode_cpp:  return "%parsercode_cpp";
   case tok_source:          return "%source"; 
   case tok_nodefaults:      return "%nodefaults"; 
   case tok_keepclosures:    return "%keepclosures"; 
   case tok_selfcheck:       return "%selfcheck"; 
   case tok_redsequence:     return "%redsequence"; 
   case tok_rules:           return "%rules";
   case tok_requires:        return "%requires";
   case tok_reduces:         return "%reduces";
   case tok_usererror:       return "%usererror";

   case tok_recover:         return "recover";

   case tok_lpar:            return "lpar";
   case tok_rpar:            return "rpar";
   case tok_lbrace:          return "lbrace";
   case tok_rbrace:          return "rbrace";
   case tok_lbracket:        return "lbracket";
   case tok_rbracket:        return "rbracket";

   case tok_lt:              return "lt";
   case tok_gt:              return "gt";
   case tok_separator:       return "separator";
   case tok_star:            return "star";
   case tok_ampersand:       return "ampersand";
   case tok_comma:           return "comma";
   case tok_dot:             return "dot"; 
   case tok_identifier:      return "identifier";
   case tok_unsigned:        return "unsigned";
   case tok_double:          return "double";
   case tok_wrongdouble:     return "wrongdouble"; 
   case tok_string:          return "string";
   case tok_wrongstring:     return "wrongstring";

   case tok_cpp_include:     return "cpp_include";
   case tok_cpp_define:      return "cpp_define";
   case tok_cpp_operator:    return "cpp_operator";

   case tok_error:           return "error";
   case tok_comment:         return "comment";
   case tok_wrongcomment:    return "wrongcomment"; 
   case tok_space:           return "space";
   case tok_empty:           return "empty"; 
   case tok_eof:             return "eof";
   }

   return "???";
}

namespace
{
   void insert( trie* tr, const char* c, token tok )
   {
      while( *c != '\0' )
      {
         auto p = tr -> rest. insert( std::pair( *c, trie( tok_error )));
         c ++;
         tr = & ( p. first -> second ); 
      }
      tr -> tok = tok;
   } 
}

void trie::insert( const char* c, token tok )
{
   ::insert( this, c, tok );
}


std::pair< token, size_t > trie::readfrom( filereader& read ) const
{
   const auto* p = this; 
   size_t i = 0;  

restart: 
   if( !read. has(i+1)) 
      return std::pair( p -> tok, i );

   auto next = p -> rest. find( read. peek(i) );

   if( next == p -> rest. end( ))
      return std::pair( p -> tok, i );

   ++ i;
   p = &( next -> second );
   goto restart;  
}


void trie::print( std::ostream& out, indentation ind ) const
{
   out << ind << "token: " << tok << "\n";
   for( const auto& p : rest )
   {
      out << ind << p. first << " :\n";
      p.second. print( out, ind + 3 );  
   }
}


std::string_view tokenize::conv2view( filereader& read, size_t len )
{
   return read. view( len ); 
}

namespace
{
   trie maph_special( )
   {
      trie spec;
 
      spec. insert( "{", tok_lbrace );
      spec. insert( "}", tok_rbrace );

      spec. insert( "::", tok_separator );
     
      spec. insert( "=>", tok_rewrite ); 
      spec. insert( "|",  tok_bar );
      spec. insert( ":",  tok_colon );
      spec. insert( ";",  tok_semicolon );
 
      return spec; 
   }

   bool starts_ident( char c )
   {
      return isalpha(c);
   }

   bool continues_ident( char c )
   {
      return isalpha(c) || isdigit(c) || c == '_';
   }

}

std::pair< token, size_t > tokenize::read_whitespace( filereader& read )
{
   if( read. has(1) && isspace( read. peek(0)) )
   {
      size_t i = 1;
      while( read. has(i+1) && isspace( read. peek(i)) )
         ++ i;
      return std::pair( tok_space, i );
   }

   return std::pair( tok_empty, 0 );
}

std::pair< token, size_t > tokenize::read_unsigned( filereader& read )
{
   if( read. has(1))
   {
      char c = read. peek(0);
      if( c == '0' )
         return std::pair( tok_unsigned, 1 );

      size_t i = 1;
      while( read. has(i+1) && isdigit( read. peek(i)) && i < 9 )
         ++ i;
      return std::pair( tok_unsigned, i );
   }

   return std::pair( tok_empty, 0 );
}

std::pair< token, size_t > tokenize::read_double( filereader& read )
{
   if( read. has(1) && 
       ( isdigit( read. peek(0)) ||
         ( read. peek(0) == '.' && read. has(2) && isdigit( read. peek(1)) )) )
   {
      size_t i = 0; 
     
      while( read. has(i+1) && isdigit( read. peek(i)) )
         ++ i;

      // fraction?

      if( read. has(i+1) && read. peek(i) == '.' )
      {
         ++ i;
         while( read. has(i+1) && isdigit( read. peek(i)) )
            ++ i;
      }

      // Exponent, no more than 3 digits.

      if( read. has(i+1) &&
            ( read. peek(i) == 'e' || read. peek(i) == 'E' ))
      {
         ++ i;
         if( read. has(i+1) && 
                ( read. peek(i) == '+' || read. peek(i) == '-' ))
         {
            ++ i;
         }

         if( !read. has(i+1) || !isdigit( read. peek(i)) )
            return std::pair( tok_wrongdouble, i ); 
         ++ i;
         if( read. has(i+1) && isdigit( read. peek(i)) )
            ++ i;
         if( read. has(i+1) && isdigit( read. peek(i)) )
            ++ i;
         if( read. has(i+1) && isdigit( read. peek(i)) )
            return std::pair( tok_wrongdouble, i + 1 ); 

      }
      return std::pair( tok_double, i ); 
   }
   return std::pair( tok_empty, 0 );
}


std::pair< token, size_t > tokenize::read_identifier( filereader& read )
{
   if( read. has(1) && starts_ident( read. peek(0)) )
   {
      size_t i = 1;
      while( read. has(i+1) && continues_ident( read. peek(i) ))
         ++ i;
      return std::pair( tok_identifier, i );
   }
   else
      return std::pair( tok_empty, 0 );
}


std::pair< token, size_t > tokenize::read_comment( filereader& read )
{
   if( read.has(1) && read. peek(0) == '/' )
   { 
      if( read. has(2))
      {
         // Starting with '//' :

         if( read. peek(1) == '/' )
         {
            size_t i = 2;
            while( read. has(i+1) && read. peek(i) != '\n' )
               ++ i;
            if( read. has(i+1))
               ++ i;             // Skip the '\n' too if it is there.
   
            return std::pair( tok_comment, i ); 
         }

         // Starting with '/*' :
         
         if( read. peek(1) == '*' ) 
         {
            size_t i = 2;
            while( read. has(i+2) &&
                   ( read. peek(i) != '*' || read. peek(i+1) != '/' ))
            {
               ++ i;
            }
 
            // This means that we encountered eof in the comment:

            if( !read. has( i + 1 ))
               return std::pair( tok_wrongcomment, i );
            if( !read. has( i + 2 ))
               return std::pair( tok_wrongcomment, i + 1 );

            return std::pair( tok_comment, i + 2 );
         }
      }
   }
   return std::pair( tok_empty, 0 ); 
}


std::pair< token, size_t > tokenize::read_string( filereader& read )
{
   if( read. has(1))
   {
      char q = read. peek(0);
      if( q == '\'' || q == '\"' )
      {
         size_t i = 1;
        
         while( read. has(i+1) && 
                read. peek(i) != q && read. peek(i) != '\n' )
         {
            if( read. peek(i) == '\\' )
            {
               if( !read. has(i+2))
                  return std::pair( tok_wrongstring, i + 1 );
               if( read. peek(i+1) == '\n' )
                  return std::pair( tok_wrongstring, i + 1 );
               i += 2;
            }
            else
               ++ i;
         } 
         if( !read.has(i+1))
            return std::pair( tok_wrongstring, i );
         if( read. peek(i) != q )
            return std::pair( tok_wrongstring, i + 1 );
         return std::pair( tok_string, i + 1 ); 
      }
   }
   return std::pair( tok_empty, 0 ); 
}

std::pair< token, size_t > tokenize::maph( filereader& read )
{

restart: 
   if( !read. has(1))
      return std::pair( tok_eof, 0 );

   if( isspace( read. peek(0)) ) 
   {
      read. commit(1);
      goto restart; 
   }

   auto p = read_comment( read );
   if( p. second ) 
   {
      read. commit( p. second );
      goto restart;
   }

   p = read_identifier( read );
   if( p. second ) 
      return p;

   static trie special = maph_special( );

   p = special. readfrom( read );

   if( p. second ) 
      return p;

   if( read. peek(0) == '%' && read. has(2) && starts_ident( read. peek(1)) )
   {
      size_t i = 2;
      while( read. has(i+1) && continues_ident( read. peek(i) ))
         ++ i;

      if( read. view(i) == "%symbol" )
         return std::pair( tok_symbol, i );
      if( read. view(i) == "%startsymbol" )
         return std::pair( tok_startsymbol, i );
      if( read. view(i) == "%infotype" )
         return std::pair( tok_infotype, i ); 
      if( read. view(i) == "%parameter" )
         return std::pair( tok_parameter, i );
      if( read. view(i) == "%symbolspace" )
         return std::pair( tok_symbolspace, i );
      if( read. view(i) == "%parserspace" )
         return std::pair( tok_parserspace, i );
      if( read. view(i) == "%symbolcode_h" )
         return std::pair( tok_symbolcode_h, i );
      if( read. view(i) == "%symbolcode_cpp" )
         return std::pair( tok_symbolcode_cpp, i );
      if( read. view(i) == "%parsercode_h" ) 
         return std::pair( tok_parsercode_h, i );
      if( read. view(i) == "%parsercode_cpp" )
         return std::pair( tok_parsercode_cpp, i );
      if( read. view(i) == "%source" )
         return std::pair( tok_source, i );
      if( read. view(i) == "%nodefaults" )
         return std::pair( tok_nodefaults, i );
      if( read. view(i) == "%keepclosures" )
         return std::pair( tok_keepclosures, i );
      if( read. view(i) == "%selfcheck" )
         return std::pair( tok_selfcheck, i ); 
      if( read. view(i) == "%redsequence" )
         return std::pair( tok_redsequence, i );
      if( read. view(i) == "%rules" )
         return std::pair( tok_rules, i );
      if( read. view(i) == "%requires" )
         return std::pair( tok_requires, i );
      if( read. view(i) == "%reduces" )
         return std::pair( tok_reduces, i );
      if( read. view(i) == "%usererror" )
         return std::pair( tok_usererror, i );

      return std::pair( tok_error, i ); 
   } 

   if( read. peek(0) == '_' ) 
   {
      char recover[] = "_recover_";

      size_t i = 1;
      while( read.has(i+1) && recover[i] && recover[i] == read. peek(i) ) 
         ++ i;

      if( recover[i] ) 
         return std::pair( tok_error, i );
      else
         return std::pair( tok_recover, i ); 
   }
 
   return std::pair( tok_empty, 0 );
      // Nothing could be read.

}


namespace
{

   trie cpp_special( ) 
   {
      trie spec;

      spec. insert( "(", tok_lpar );
      spec. insert( ")", tok_rpar );
      spec. insert( "{", tok_lbrace );
      spec. insert( "}", tok_rbrace );
      spec. insert( "[", tok_lbracket );
      spec. insert( "]", tok_rbracket ); 

      spec. insert( "::", tok_separator );
      spec. insert( "<", tok_lt );
      spec. insert( ">", tok_gt );
      spec. insert( "*", tok_star );
      spec. insert( "&", tok_ampersand );

      spec. insert( ":", tok_colon );
      spec. insert( ";", tok_semicolon );

      spec. insert( ",", tok_comma );
      spec. insert( ".", tok_dot );

      spec. insert( "+", tok_cpp_operator );
      spec. insert( "++", tok_cpp_operator );
      spec. insert( "-", tok_cpp_operator );
      spec. insert( "--", tok_cpp_operator );
      spec. insert( "/", tok_cpp_operator );
      spec. insert( "%", tok_cpp_operator );
      spec. insert( "?", tok_cpp_operator ); 
      spec. insert( "!=", tok_cpp_operator );
      spec. insert( "|", tok_cpp_operator );
    
      spec. insert( "=", tok_cpp_operator );

      // All the other operators are just 'operator'.

      return spec; 
   }
}


std::pair< token, size_t > tokenize::cpp( filereader& read )
{
   if( !read. has(1))
      return std::pair( tok_eof, 0 );

   auto p = read_whitespace( read );
   if( p. second ) 
      return p; 
   
   p = read_identifier( read );
   if( p. second ) 
      return p;

   p = read_double( read );
   if( p. second ) 
      return p;

   p = read_comment( read );
   if( p. second ) 
      return p;

   p = read_string( read );
   if( p. second ) 
      return p;

   if( read. peek(0) == '#' && read. has(2) ) 
   {
      size_t i = 1;
      while( read. has(i+1) && continues_ident( read. peek(i) ))
         ++ i;
      
      if( read. view(i) == "#include" )
         return std::pair( tok_cpp_include, i ); 
      if( read. view(i) == "#define" ) 
         return std::pair( tok_cpp_define, i ); 
      
      return std::pair( tok_error, i ); 
   }

   static trie spec = cpp_special( );

   p = spec. readfrom( read );

   if( p. second ) 
      return p;

   return std::pair( tok_empty, 0 );
}


