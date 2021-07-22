
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

#ifndef MAPH_PARSERCODEBUILDER_ 
#define MAPH_PARSERCODEBUILDER_  1 

#include "symbolcodebuilder.h"
#include "parsetable.h"

// This struct has an open design because it is gradually filled
// in with user input.

struct parsercodebuilder 
{

   cpp::nspace space;   // Full name of the parser always ends in 'parser'.

   cpp::codefragment source;
      // This must be C++ expression that can be used as source.
      // It must return a symbol. 

   std::vector< std::pair< cpp::identifier, cpp::type >> parameters;
      // Order matters, so we cannot use a map here. 

   cpp::codefragment code_h;
   cpp::codefragment code_cpp;
      // This is code that goes to the beginning of parser.h 
      // (intended for includes), and code that goes to the
      // beginning of parser.cpp (intended for helper functions in
      // anonymous namespace).

   std::vector< symbol::map< size_t >> redsequences;
      // If two symbols occur in the same redsequences[i], then
      // the one which is mapped to the lower number will be preferred.
  
   bool usedefaults = true; 
      // If true, we use default reductions. This simplifies
      // the parser, and is useful for interactive applications.

   bool usererror = false;
      // If true, the user defines his own error function.
      // If fase, we create a default one.
  
   // Code that is associated to a reduction:

   struct reduction
   {
      std::unordered_map< std::string, size_t > parameters;
         // These are the parameters that the user defined in the 
         // reduction code. For example E => E:e plus F:f will result in
         // parameters[ "e" ] = 0, and parameters[ "f" ] = 2.
         // The types can be read off from the rules, so we don't store them.

      cpp::codefragment cond;
      cpp::codefragment body;

      reduction( std::unordered_map< std::string, size_t > && parameters, 
                 cpp::codefragment&& cond,
                 cpp::codefragment&& body )
         : parameters( std::move( parameters )),
           cond( std::move( cond )),
           body( std::move( body ))
      { }

      bool empty( ) const { return cond. empty( ) && body. empty( ); }

      void print( std::ostream& out ) const; 
   };

   grammar grrr;
   std::vector< reduction > reductions; 

   parsercodebuilder( const cpp::nspace& space )
      : space( space )
   { }


   // A sequence of pointers to rules. They represent reduction
   // candidates that will be tried in the order of appearance. 

   struct rulesequence 
   {
      std::vector< const rule* > seq;

      rulesequence( ) = default;

      void append( const rule* r ) 
         { seq. push_back(r); }

      bool lhs_containedin( const symbol::map< size_t > & set ) const;
         // True of the lhs-s of all our rules are contained
         // in the domain of set.

      template< typename P > 
      P first_lhs_containedin( P p1, P p2 ) const 
      {
         while( p1 != p2 )
         {
            if( lhs_containedin( *p1 ))
               return p1;
            ++ p1;
         }
         return p2;
      }

      void sort_with( const symbol::map< size_t > & pref ); 
         // Sort one the value of pref[ ] of the lhs, 
         // putting lower pref[ ] first. Every lhs encountered
         // must be defined in pref, otherwise we crash.  

      using const_iterator = std::vector< const rule* > :: const_iterator;
      const_iterator begin( ) const { return seq. begin( ); } 
      const_iterator end( ) const { return seq. end( ); } 

      size_t size( ) const { return seq. size( ); }

      void print( std::ostream& out ) const; 
   };
 
   void 
   printcode( const symbolcodebuilder& bld_sym, const parsetable& pt,
              std::vector< conflict > & conflicts,
              std::istream& idee, std::ostream& cpp, std::ostream& hh ) const;
      // Uses idee.x as template.
      // We need the symbolcode builder for the namespace of symbol,
      // for the types of the symbols and for the attribute type. 

   std::unordered_map< std::string, std::string >
   createreplacement( size_t nr, const symbolcodebuilder& bld_sym ) const;
      // Create the replacment that maps the parameters 
      // for the i-th rule into their expressions.

   void printparameterfields( std::ostream& ) const;
   void printconstructor( std::ostream& ) const; 
   void printactiondeclarations( 
           const symbolcodebuilder& bld_sym, std::ostream& ) const;
   void printactiondefinitions(
           const symbolcodebuilder& bld_sym, std::ostream& ) const;

   void printmaintables( const parsetable& pt, 
                         const cpp::nspace& symbolspace,
                         std::vector< conflict > & conflicts, 
                         std::ostream& out ) const;

   void printreductioncases( const symbolcodebuilder& bld_sym, 
                             std::ostream& out ) const; 
};

std::ostream& operator << ( std::ostream& out, const parsercodebuilder& bld );

#endif


