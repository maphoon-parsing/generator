
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


#include "tokenizing.h"

namespace
{

   bool starts_ident( char c )
   {
      return isalpha(c);
   }

   bool continues_ident( char c )
   {
      return isalpha(c) || isdigit(c) || c == '_';
   }

}


std::pair< symboltype, size_t > 
tokenizing::try_whitespace( filereader& read )
{
   if( read. has(1) && isspace( read. peek(0)) )
   {
      size_t i = 1;
      while( read. has(i+1) && isspace( read. peek(i)) )
         ++ i;
      return std::pair( sym_WHITESPACE, i );
   }

   return std::pair( sym_EMPTY, 0 );
}


std::pair< symboltype, size_t > 
tokenizing::try_double( filereader& read )
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
            return std::pair( sym_SCANERROR, i );
         ++ i;
         if( read. has(i+1) && isdigit( read. peek(i)) )
            ++ i;
         if( read. has(i+1) && isdigit( read. peek(i)) )
            ++ i;
         if( read. has(i+1) && isdigit( read. peek(i)) )
            return std::pair( sym_SCANERROR, i + 1 );

      }
      return std::pair( sym_DOUBLE, i );
   }
   return std::pair( sym_EMPTY, 0 );
}


std::pair< symboltype, size_t > 
tokenizing::try_identifier( filereader& read )
{
   if( read. has(1) && starts_ident( read. peek(0)) )
   {
      size_t i = 1;
      while( read. has(i+1) && continues_ident( read. peek(i) ))
         ++ i;
      return std::pair( sym_IDENTIFIER, i );
   }
   else
      return std::pair( sym_IDENTIFIER, 0 );
}

std::pair< symboltype, size_t > 
tokenizing::try_comment( filereader& read )
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

            return std::pair( sym_IDENTIFIER, i );
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
               return std::pair( sym_SCANERROR, i );
            if( !read. has( i + 2 ))
               return std::pair( sym_SCANERROR, i + 1 );

            return std::pair( sym_COMMENT, i + 2 );
         }
      }
   }
   return std::pair( sym_EMPTY, 0 );
}


symbol tokenizing::readsymbol( filereader& read ) 
{

restart:
   if( !read. has(1))
   {
      return symbol( sym_EOF, 
                     location( &read. filename, read. line, read. column ));
   }

   auto p = try_whitespace( read );
   if( p. second ) 
   {
      read. commit( p. second );
      goto restart;
   }

   p = try_comment( read );
   if( p. second ) 
   {
      read. commit( p. second );
      goto restart;
   }

   location loc( &read. filename, read. line, read. column );

   p = try_double( read );
   if( p. second )  
   {
      double val = std::stod( std::string( read. view( p. second ))); 
      read. commit( p. second ); 
      return symbol( sym_DOUBLE, loc, val );
   }
  
   p = try_identifier( read );
   if( p. second )
   { 
      std::string id = std::string( read. view( p. second ));
      read. commit( p. second );
      return symbol( sym_IDENTIFIER, loc, std::move( id )); 
   }

   char c = read. peek(0);

   // # can be used as end-of-file:

   if( c == '#' )
   {
      // We don't commit. 

      return symbol( sym_EOF, loc );
   }

   if( c == ';' )
   {
      read. commit(1);
      return symbol( sym_SEMICOLON, loc );
   }

   if( c == ':' && read. has(2) && read. peek(1) == '=' ) 
   {
      read. commit(2);
      return symbol( sym_BECOMES, loc );
   }

   if( c == ',' ) 
   {
      read. commit(1); 
      return symbol( sym_COMMA, loc );
   }

   if( c == '+' )
   {
      read. commit(1);
      return symbol( sym_PLUS, loc );
   }

   if( c == '-' ) 
   {
      read. commit(1);
      return symbol( sym_MINUS, loc );
   }

   if( c == '*' )
   {
      read. commit(1);
      return symbol( sym_TIMES, loc );
   }

   if( c == '/' ) 
   {
      read. commit(1);
      return symbol( sym_DIVIDES, loc ); 
   }

   if( c == '%' )
   {
      read. commit(1);
      return symbol( sym_MODULO, loc );
   }

   if( c == '!' )
   {
      read. commit(1);
      return symbol( sym_FACTORIAL, loc );
   }

   if( c == '(' )
   {
      read. commit(1);
      return symbol( sym_LPAR, loc );
   }

   if( c == ')' )
   {
      read. commit(1);
      return symbol( sym_RPAR, loc );
   }

   // If we could not recognize anything, then we produce
   // a scan error.

   std::string err; err. push_back(c);

   read. commit(1);
   return symbol( sym_SCANERROR, loc, err );
}


