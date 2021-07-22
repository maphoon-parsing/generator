
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


// Written by Hans de Nivelle, spring 2021.

#include "ourownparser.h" 
#include "errors.h"

void ourownparser::maph_forward( )
{
   read. commit( lookahead. second );
   lookahead = tokenize::maph( read );
}

void ourownparser::cpp_forward( )
{
   read. commit( lookahead. second );
   lookahead = tokenize::cpp( read );
}

void ourownparser::throwsyntaxerror( const std::string& problem )
{
   throw syntaxerror( problem, read.filename, read.line, read.column );
}

void ourownparser::throwgeneralerror( const std::string& problem )
{
   throw generalerror( problem, read.filename, read.line, read.column );
}

std::string_view ourownparser::conv2view( ) 
{
   return read. view( lookahead. second );
}


std::vector< std::string > ourownparser::readcpptype( )
{
   std::vector< std::string > tp; 

   if( lookahead. first != tok_lbrace ) 
      throw std::runtime_error( "lookahead must be { " );

   cpp_forward( );
 
   size_t depth = 1;
      // Nesting depth of { and }. We already have read one { .

   while( true )
   {
      switch( lookahead. first ) 
      {
      case tok_rbrace:
         -- depth;
         if( depth == 0 )
         {
            maph_forward( ); 
            return tp; 
         }
         cpp_forward( ); 
         tp. push_back( "}" );       
         break;

      case tok_lbrace:
         ++ depth;
         tp. push_back( "{" );
         cpp_forward( );
         break; 

      case tok_space:
      case tok_comment: 
         cpp_forward( ); 
         break;

      case tok_identifier:
      case tok_separator:
      case tok_lt:
      case tok_gt:
      case tok_star:
      case tok_ampersand: 
      case tok_comma: 
         tp. push_back( std::string( read. view( lookahead. second )));
         cpp_forward( ); 
         break; 
       
      default:
         throwsyntaxerror( std::string( getcstring( lookahead.first )) +
                           " " + 
			   std::string( conv2view( )) + 
                           " not allowed in C++ type" );

      }
   }
}


void ourownparser::readheadercode( cpp::codefragment& header ) 
{
   if( lookahead. first != tok_lbrace )
      throw std::runtime_error( "lookahead must be { " );

   cpp_forward( );

   size_t depth = 1;
      // Nesting depth of { and }. We already have read one { .

   while( true )
   {
      switch( lookahead. first )
      {
      case tok_rbrace:
         -- depth;
         if( depth == 0 )
         {
            maph_forward( );
            return;
         }
         cpp_forward( );
         header. push_back( "}" );
         break;

      case tok_lbrace:
         ++ depth;
         header. push_back( "{" );
         cpp_forward( );
         break;

      case tok_lpar:
      case tok_rpar:

      case tok_space:
      case tok_comment:
         header. push_back( conv2view( )); 
         cpp_forward( );
         break;

      case tok_lt:
      case tok_gt:
      case tok_cpp_include:
      case tok_string: 
      case tok_identifier: 
      case tok_comma:
      case tok_dot: 
      case tok_separator:
      case tok_colon:
      case tok_semicolon:
      case tok_star:
      case tok_ampersand:
         header. push_back( conv2view( ));
         cpp_forward( );
         break;

      default:
         throwsyntaxerror( std::string( "forbidden use of " ) + 
                           std::string( read. view( lookahead. second ))
                            + " in header code" );
      }
   }
}


void ourownparser::readstandardcode( cpp::codefragment& code ) 
{
   unsigned int startline = read.line;
   unsigned int startcolumn = read.column;

   if( lookahead. first != tok_lbrace ) 
      throw std::runtime_error( "lookahead must be { " );

   cpp_forward( );
 
   size_t depth = 1;
      // Nesting depth of { and }. We have already read one { .

   while( true )
   {
      switch( lookahead. first ) 
      {
      case tok_rbrace:
         -- depth;
         if( depth == 0 )
         {
            maph_forward( ); 
            return; 
         }
         cpp_forward( ); 
         code. push_back( "}" );       
         break;

      case tok_lbrace:
         ++ depth;
         code. push_back( "{" );
         cpp_forward( );
         break; 

      case tok_space:
      case tok_comment: 
         code. push_back( std::string( conv2view( ))); 
         cpp_forward( ); 
         break;

      case tok_identifier:
         code. push_back( std::string( conv2view( )), true );
         cpp_forward( );
         break;

      case tok_lpar:
      case tok_rpar:
      case tok_lbracket:
      case tok_rbracket:
         code. push_back( std::string( conv2view( )));
         cpp_forward( );
         break;

      case tok_string:
      case tok_double: 
         code. push_back( std::string( conv2view( )));
         cpp_forward( );
         break;

      case tok_separator:
      case tok_lt:
      case tok_gt:
      case tok_star:
      case tok_ampersand: 
      case tok_comma: 
      case tok_dot: 
      case tok_colon:
      case tok_semicolon:
      case tok_cpp_operator: 
         code. push_back( std::string( conv2view( )));
         cpp_forward( ); 
         break; 
       
      default:
         throwsyntaxerror( std::string( getcstring( lookahead.first )) + 
                           " " + 
                           std::string( conv2view( )) + 
                           " not allowed in code block (starting at " + 
                           std::to_string(startline+1) + "/" + 
                           std::to_string(startcolumn+1) + ")" );

      }
   }
}

cpp::nspace ourownparser::readspace( )
{
   std::vector< std::string > res;

   if( lookahead. first != tok_identifier ) 
      return res;

   res. push_back( std::string( conv2view( )));
   maph_forward( );

   while( lookahead. first == tok_separator )
   {
      maph_forward( );
      if( lookahead. first != tok_identifier )
         throwsyntaxerror( "expecting identifier for namespace" );
      
      res. push_back( std::string( conv2view( )));
      maph_forward( ); 
   }

   return res; 
}

void ourownparser::startsymbol( grammar& gr )
{ 
   if( lookahead. first != tok_startsymbol )
      throw std::runtime_error( "lookahead must be startsym" );

   maph_forward( );

   if( lookahead. first != tok_identifier )
      throwsyntaxerror( "identifier expected" ); 

   symbol startsym = std::string( conv2view( ));
   maph_forward( );
 
   symbolset follows;
   while( lookahead. first == tok_identifier )
   {
      follows. insert( symbol( std::string( conv2view( )))); 
      maph_forward( );
   }

   gr. addstartsym( startsym, follows );
}

void ourownparser::infotype( cpp::type& tp )
{
   if( lookahead. first != tok_infotype )
      throw std::runtime_error( "should be infotype" );

   maph_forward( );
   if( lookahead. first != tok_lbrace )
      throwsyntaxerror( "left { expected: " );

   std::vector< std::string > str = readcpptype( );
   if( str. size( ) == 0 )
      str. push_back( "void" );
 
   tp = std::move( str );
}

void ourownparser::symbols( symbolcodebuilder& bld_sym )
{
   if( lookahead. first != tok_symbol )
      throw std::runtime_error( "should have been symbol" );

   maph_forward( ); 

   std::vector< std::string > str;

   if( lookahead. first == tok_lbrace )
      str = readcpptype( );

   if( str. size( ) == 0 )
      str. push_back( "void" );
   
   if( lookahead. first != tok_identifier )
      throwsyntaxerror( "there should be at least one declared symbol" );

   cpp::type thetype = str;

   while( lookahead. first == tok_identifier )
   {
      if( !bld_sym. addsymbol( std::string( conv2view( )), thetype ))
      {
         throwsyntaxerror( std::string( "redeclaration of symbol " ) +
                           std::string( conv2view( )) );
      }

      maph_forward( ); 
   }
}

void ourownparser::parameter( parsercodebuilder& bld_parse )
{
   if( lookahead. first != tok_parameter )
      throw std::runtime_error( "should have been parameter" );

   maph_forward( );

   if( lookahead. first != tok_lbrace )
      throwsyntaxerror( "parameters cannot be declared without type" );

   std::vector< std::string > str = readcpptype( ); 
   if( str. size( ) == 0 )
      str. push_back( "void" );

   if( lookahead. first != tok_identifier )
      throwsyntaxerror( "parameter( identifier ) expected" );

   bld_parse. parameters. push_back( 
      std::pair( std::string( conv2view( )), cpp::type( str ))); 

   maph_forward( );
}


void ourownparser::code_h( cpp::codefragment& code_h ) 
{
   if( code_h. size( ) == 0 )
   {
      code_h. filename = read. filename;
      code_h. line = read. line;
      code_h. column = 0;
   }

   if( lookahead. first != tok_lbrace )
      throwsyntaxerror( "left { expected: " );

   readheadercode( code_h );
   code_h. push_back( "\n" );
}


void ourownparser::code_cpp( cpp::codefragment& code_cpp )
{
   if( code_cpp. size( ) == 0 )
   {
      code_cpp. filename = read. filename;
      code_cpp. line = read. line;
      code_cpp. column = 0;
   }

   if( lookahead. first != tok_lbrace )
      throwsyntaxerror( "left { expected: " );

   readstandardcode( code_cpp );
   code_cpp. push_back( "\n" );
}


void ourownparser::rulegroup( parsercodebuilder& bld_parse )
{
   if( lookahead. first != tok_identifier )
      throw std::runtime_error( "grammar rule must start with lhs" );

   symbol lhs = std::string( conv2view( ));
   size_t nrrules = 0; 
      // nr of rules that share this lhs.

   maph_forward( );
   while( lookahead. first != tok_semicolon )
   {
      if( nrrules == 0 && lookahead. first != tok_rewrite )
      {
         throwsyntaxerror( "=> expected" );
      }

      if( nrrules != 0 &&
          lookahead. first != tok_rewrite &&
          lookahead. first != tok_bar )
      { 
         throwsyntaxerror( "| or => expected" );
      }

      maph_forward( );

      std::vector< symbol > rhs;
      std::unordered_map< std::string, size_t > parameters;

      while( lookahead. first == tok_identifier ||
             lookahead. first == tok_recover ) 
      {
         symbol right = std::string( conv2view( ));
         maph_forward( );

         if( lookahead. first == tok_colon )
         {
            maph_forward( );
            if( lookahead. first != tok_identifier )
            {
               throwsyntaxerror( "attribute identifier expected" );
            }

            auto p = parameters. insert( 
                        std::pair( std::string( conv2view( )), rhs. size( )));     

            if( !p. second )
            {
               throwgeneralerror( std::string( "attribute name " ) + 
                                  std::string( conv2view( )) + 
                                  std::string( " is reused" ));
            }
            maph_forward( );
         }

         rhs. push_back( right );
      }

      cpp::codefragment cond;
      
      if( lookahead. first == tok_requires )
      {
         maph_forward( );
         if( lookahead. first != tok_lbrace )
            throwsyntaxerror( "expecting { after %requires" );
 
         cond. filename = read. filename;
         cond. line = read. line;
         cond. column = read. column;

         readstandardcode( cond ); 
      }
 
      cpp::codefragment body;

      if( lookahead. first == tok_reduces ) 
         maph_forward( );

      if( lookahead. first == tok_lbrace ) 
      {
         body. filename = read. filename;
         body. line = read. line;
         body. column = read. column;

         readstandardcode( body ); 
      }

      if( lookahead. first != tok_bar &&
          lookahead. first != tok_rewrite &&
          lookahead. first != tok_semicolon )
      {
         throwsyntaxerror( 
            std::string( "unexpected symbol " ) + 
            std::string( conv2view( )) +
            " in rhs of grammar rule" );
      }
 
      bld_parse. grrr. push_back( 
            rule( bld_parse. grrr. nrrules( ), lhs, std::move( rhs )));

      bld_parse. reductions. emplace_back( std::move( parameters ),
                                           std::move( cond ),
                                           std::move( body ));
      ++ nrrules;
   }

   maph_forward( );   // We know it's a semicolon.

}


symbol::map< size_t > ourownparser::redsequence( )
{
   if( lookahead. first != tok_redsequence )
      throw std::runtime_error( "first should have been redsequence" );

   maph_forward( );

   symbol::map< size_t > result;

   while( lookahead. first == tok_identifier )
   {
      symbol s = std::string( conv2view( ));

      if( !result. insert( { s, result. size( ) } ). second ) 
      {
         throwgeneralerror( std::string( "symbol " ) +
                            std::string( conv2view( )) + 
                            " occurs twice in same reduction sequence" );    
      }

      maph_forward( );  
   }

   return result;
}

void ourownparser::print( std::ostream& out ) const
{
   out << read;
   out << "lookahead: " << lookahead << "\n";
}

