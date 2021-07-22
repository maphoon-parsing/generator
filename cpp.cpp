
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


// Written by Hans de Nivelle.

#include "cpp.h"


bool cpp::type::islight( ) const
{
   // If we are a pointer, we are light:

   if( repr. size( ) >= 2 && repr. back( ) == "*" )
      return true;

   if( repr. size( ) == 1 && repr[0] == "void" )
      return true;

   size_t i = 0;
   while( i < repr. size( ) && 
          ( repr[i] == "short" || repr[i] == "long" || 
            repr[i] == "unsigned" || repr[i] == "signed" ))
   {
      ++ i;
   }

   if( i == repr. size( ))
      return true;     // Only things like short/long/unsigned/signed. 
 
   if( i + 1 == repr. size( ))
   { 
      return repr[i] == "bool" || repr[i] == "char" || 
             repr[i] == "int" || repr[i] == "float" ||
             repr[i] == "double" || repr[i] == "size_t"; 
   }

   return false; 
}

bool cpp::type::isconst( ) const
{
   return repr. size( ) >= 2 && repr[0] == "const"; 
}

bool cpp::type::isref( ) const
{
   return repr. size( ) >= 2 && repr. back( ) == "&";
}

size_t cpp::type::hash::operator( ) ( const type& tp ) const
{
   std::hash< std::string > h;
   size_t hv = 0;
   for( size_t i = 0; i != tp. repr. size( ); ++ i )
   {
      hv = hv * 17 + h( tp. repr[i] );
         // not great, but good enough. 
   }
   return hv; 
}

bool 
cpp::type::equal_to::operator( ) ( const type& tp1, const type& tp2 ) const
{
   if( tp1. repr. size( ) != tp2. repr. size( ))
      return false; 

   for( size_t i = 0; i != tp1. repr. size( ); ++ i )
   {
      if( tp1. repr[i] != tp2. repr[i] )
         return false; 
   }
   return true; 
}

namespace
{
   // True if no space between s1 and s2 would merge s1 and s2
   // into a single C++ token. 

   bool spacebetween( const std::string& s1, const std::string& s2 )
   {
      if( s1. size( ) == 0 || s2. size( ) == 0 )
         return true;

      if( s1. back( ) == ',' ) 
         return true;

      bool a1 = isalpha( s1. back( )) || isdigit( s1. back( ));
      bool a2 = isalpha( s2. front( )) || isalpha( s2. front( ));

      if( a1 && !a2 ) return false;
      if( !a1 && a2 ) return false;

      return true;
   }
}

void cpp::type::print( std::ostream& out ) const
{
   for( size_t i = 0; i != repr. size( ); ++ i )
   {
      if( i != 0 && spacebetween( repr[i-1], repr[i] ))
         out << ' ';
      out << repr[i];
   }
}

void cpp::type::printascode( std::ostream& out, cv_ref r ) const
{
   switch(r)
   {
      case value: print( out ); return; 
      case ref: print( out );  out << " &"; return;
      case cref: out << "const ";  print( out ); out << " &"; return;
      case mref: print( out );  out << " &&"; return;
   }
}


namespace
{
   bool seemssafe( const std::string& s )
   {
      if( s. size( ) == 0 || !isalpha( s[0] )) return false; 
      
      for( size_t i = 1; i != s. size( ); ++ i )
      {
         if( !isalpha( s[i] ) && !isdigit( s[i] ) && s[i] != '_' )
            return false;
      }

      // Finally, we compare against some reserved identifiers:

      const static std::unordered_set< std::string > reserved = 
      { "if", "else", "for", "while", "do", "return", 
        "switch", "case", "break", "continue", "default", "goto",
        "throw", "catch", "main", "this",
        "void", "bool", "char", "int", "float", "double", 
        "long", "short", "signed", "unsigned", 
        "enum", 
        "inline", "sizeof", 
        "false", "true", 
        "class", "struct", "union", "public", "private", "protected", 
        "const", "volatile", "virtual", "override", 
        "namespace", "using", "typedef"  
      };

      if( reserved. count(s) )
         return false;  

      return true; 
   }
}

bool cpp::identifier::seemssafe( ) const
{
   return ::seemssafe( repr ); 
}


bool cpp::nspace::seemssafe( ) const
{
   for( const auto& s : repr )
   {
      if( ! ::seemssafe(s))
         return false; 
   }
   return true; 
}


void cpp::nspace::printasprefix( std::ostream& out ) const 
{
   for( const auto& s : repr ) 
      out << s << "::"; 
}


void cpp::nspace::open( std::ostream& out ) const

{
   for( const auto& n : repr )
      out << "namespace " << n << " { ";
   if( repr. size( ))
      out << "\n\n";
}


void cpp::nspace::close( std::ostream& out ) const
{
   for( const auto& n : repr )
      out << "}";
   if( repr. size( ))
      out << "\n";
}


std::string 
cpp::nspace::includeguard( const std::string& filename ) const 
{
   std::string guard; 

   for( const auto& n : repr ) 
   {
      for( char c : n )
         guard. push_back( toupper( c )); 
      guard. push_back( '_' );
   }
 
   for( char c : filename )
      guard. push_back( toupper(c));
 
   return guard;
}


void cpp::nspace::print( std::ostream& out ) const
{
   for( size_t i = 0; i != size( ); ++ i ) 
   {
      if(i) out << " :: ";
      out << repr[i];
   }
}


#if 0

bool cpp::wellformedprefixed( const std::string& s )
{
   for( std::string::const_iterator 
           p = s. begin( );
           p != s. end( );
           ++ p )
   {
      if(  ( *p < 'a' || *p > 'z' ) &&
           ( *p < 'A' || *p > 'Z' ) &&
           ( *p < '0' || *p > '9' ) &&
           *p != '_' )
      {
         return false;
      }
   }

   return true;
}


bool cpp::wellformedalone( const std::vector< std::string > & n )
{
   for( auto p = n. begin( ); p != n. end( ); ++ p )
   {
      if( ! wellformedalone( *p ))
         return false;
   }
   return true;
}


std::string cpp::makedeclaration( const std::vector< std::string > & n )
{
   std::string res;

   for( auto p = n. begin( ); p != n. end( ); ++ p )
   {
      res += "namespace ";
      res += *p;
      res += " { ";
   }
   return res;
}
 
     
std::string cpp::makeclosure( const std::vector< std::string > & n )
{
   std::string res;

   for( auto p = n. begin( ); p != n. end( ); ++ p )
   {
      if( p != n. begin( ))
         res += " "; 
      res += "}"; 
   }
   return res;
}

 
std::string cpp::indentation( const std::vector< std::string > & n )
{
   std::string res;
   for( auto p = n. begin( ); p != n. end( ); ++ p )
   {
      res += "   ";
   }
   return res;
}


#endif

#if 0

std::string cpp::attribute_name( const std::string& attr )
{
   return attr;
}


std::string cpp::symbol_name( symbol s )
{
   return "tkn_" + s. getstring( );
}


std::string cpp::symbol_name( const std::vector< std::string > & n, 
                              symbol s )
{
   std::string result;

   for( auto p = n. begin( ); p != n. end( ); ++ p )
   {
      result += *p;
      result += "::";
   }

   result += symbol_name( s ); 
   return result;  
}

#endif 


///////////////////////////////////////////////////////////////////////////

void cpp::codefragment::push_back( std::string_view v, bool id ) 
{
   if( id )
   {
      size_t s = repr. size( );
      repr += v;
      
      identifiers. push_back( std::pair( s, repr. size( )) );
   }
   else
      repr += v;
}

bool cpp::codefragment::empty( ) const
{
   for( char c : repr )
   {
      if( !isspace(c))
         return false;
   }
   return true;
}

void cpp::codefragment::print( std::ostream& out ) const
{
   out << "from file " << filename << ", line " << line << " :\n"; 
   size_t pos = 0;
   size_t id = 0;
   size_t ln = line; 
   out << "   " << ( ln ++ ) << " :   ";

   while( pos != repr. size( )) 
   {
      if( id < identifiers. size( ) && identifiers[id]. first == pos )
         out << '{';
      if( id < identifiers. size( ) && identifiers[id]. second == pos )
      {
         out << '}';  
         ++ id; 
      }
      char c = repr[ pos ++ ];
      out << c;
      if( c == '\n' && pos != repr. size( ))
      {
         out << "   " << ( ln ++ ) << " :   ";
      }         
   }
   out << "\n";
}

void
cpp::codefragment::printascode( std::ostream& out ) const
{
   if( filename. size( )) 
      out << "#line " << ( line + 1 ) << " \"" << filename << "\"" << "\n";
   out << repr;
}

void cpp::codefragment::printascode( std::ostream& out, 
         const std::unordered_map< std::string, std::string > & repl ) const
{
   if( filename. size( )) 
      out << "#line " << ( line + 1 ) << " \"" << filename << "\"" << "\n";
  
   size_t pos = 0;
   for( size_t i = 0; i != identifiers. size( ); ++ i )
   {
      while( pos < identifiers[i]. first )
         out << repr[ pos ++ ];
      
      std::string id;
      while( pos < identifiers[i]. second )
         id. push_back( repr[ pos ++ ] );
      
      auto p = repl. find( id );
      if( p == repl. end( ))
         out << id;
      else
         out << ( p -> second );
   }

   while( pos != repr. size( )) 
      out << repr[ pos ++ ];
}

