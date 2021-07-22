
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


// Code written by Hans de Nivelle, June 2008.

#ifndef MAPH_CPP_
#define MAPH_CPP_  1

#include <string>
#include <vector>

#include "symbol.h"

namespace cpp 
{

   // Maybe all these types are a bit bloated, but I think it is better 
   // in long term to use a type rich programming style. 

   // Types should not contain qualiifers like 'const' or 'volatile'
   // and not be a reference. Pointers are OK. 

   struct type
   {
      std::vector< std::string > repr;  

      type( ) = delete;

      type( const std::vector< std::string > & repr )
         : repr( repr )
      { }

      type( std::vector< std::string > && repr )
         : repr( std::move( repr ))
      { }

      type( const std::string& repr )
         : type( std::vector< std::string > { repr } )
      { }

      bool isvoid( ) const
         { return repr. size( ) == 1 && repr[0] == "void"; }

      bool islight( ) const;
         // True if we are light to copy. In that case, we don't
         // create const& and && constructurs, but a simple
         // value constructor.

      bool isconst( ) const;
         // True if we start with 'const'.

      bool isref( ) const;
         // True if we end with '&' or '&&'.

      struct hash
      {
         std::hash< std::string > h;
         size_t operator( ) ( const type& t ) const;
      };

      struct equal_to
      {
         std::equal_to< std::string > eq;
         bool operator( ) ( const type& t1, const type& t2 ) const;
      };

      enum cv_ref { value, ref, cref, mref };
         // We have value, reference, const reference, moving reference.

      void print( std::ostream& out ) const; 

      void printascode( std::ostream& out, cv_ref r = value ) const;

      using set = std::unordered_set< type, hash, equal_to > ;
   }; 

   inline 
   std::ostream& operator << ( std::ostream& out , const type& type )
      { type. print( out ); return out; }  

   struct identifier
   {
      std::string repr;

      identifier( const std::string& repr )
         : repr( repr ) 
      { }

      identifier( std::string&& repr )
         : repr( std::move( repr ))
      { }

      struct hash
      {
         std::hash< std::string > h;  

         size_t operator( ) ( const identifier& id ) const
            { return h( id. repr ); }
      };

      struct equal_to
      {
         std::equal_to< std::string > eq; 

         bool operator( ) ( const identifier& id1, 
                            const identifier& id2 ) const 
            { return eq( id1. repr, id2. repr ); }
      };

      void print( std::ostream& out ) const 
         { out << repr; } 

      bool seemssafe( ) const; 
         // True if the identifier seems save to use.
         // That means: Not a reserved word and a valid identifier. 
   };

   inline 
   std::ostream& operator << ( std::ostream& out, const identifier& id )
      { id. print( out ); return out; } 


   // A C++ namespace:

   struct nspace 
   {
      std::vector< std::string > repr;

      nspace( ) = default;

      nspace( const std::vector< std::string > & repr )
         : repr( repr )
      { }

      nspace( std::vector< std::string > && repr )
         : repr( std::move( repr ))
      { }

      bool seemssafe( ) const; 
         // True if the namespace seems save to use.
         // It should consists of identifiers and not contain reserved words.

      size_t size( ) const { return repr. size( ); } 

      void printasprefix( std::ostream& out ) const; 
         // print the namespace as prefix to a name. 

      void open( std::ostream& out ) const;
      void close( std::ostream& out ) const; 
      std::string includeguard( const std::string& filename ) const;

      void print( std::ostream& out ) const;
         // Print in readable fashion. Should not be used for code
         // generation. Use printasprefix. 
   };

   inline 
   std::ostream& operator << ( std::ostream& out, const nspace& sp )
   {
      sp. print( out ); return out;
   }

 
   std::string attribute_name( const std::string& attr );
      // The string by which the attribute (of token) will be represented.
      // It is just attr. 

   std::string symbolname( symbol s );
   std::string symbolname( const std::vector< std::string > & n, symbol s ); 
      // The string by which symbol s will be represented in the
      // enumeration. It will be the string "tkn_" + s. 
      // The second variant constructs n + "tkn_ " + s. 



   // A fragment of C++ code. It is important that it is output 
   // exactly as input without changes in spacing. 

   struct codefragment
   {
      std::string filename;
      long unsigned int line; 
      long unsigned int column;      

      std::string repr;
         // The code without change. 

      std::vector< std::pair< size_t, size_t >> identifiers; 
         // Positions of replaceable identifiers in repr.
 
      codefragment( ) 
         : line(0), 
           column(0) 
      { }

      void push_back( std::string_view v, bool id = false );
         // True if v should be considered as identifier. 
         // In that case we also extend identifiers.
         // It works with std::string and const char* v too.

      void print( std::ostream& out ) const; 

      void printascode( std::ostream& out ) const; 
      void printascode( std::ostream& out, 
             const std::unordered_map< std::string, std::string > & ) const; 

         // If filename is not the empty string, we print a  
         // line directive in front of the code.

      size_t size( ) const { return repr. size( ); } 

      bool empty( ) const; 
         // True if we are empty, which means that we contain only whitespace.

   };

   inline 
   std::ostream& operator << ( std::ostream& out, const codefragment& frag )
   {
      frag. print( out ); return out;
   }
}

inline
std::ostream&
operator << ( std::ostream& out, 
              const std::unordered_map< std::string, std::string > & repl )
{
   out << "variable replacement:\n";
   for( const auto& p : repl )
   {
      out << "   " << p. first << " --> " << p. second << "\n";
   }
   return out;
}

#endif


