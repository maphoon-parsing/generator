
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


// Code written by Hans de Nivelle, November 2006.
// Simplified, June 2008.
// Rewritten April 2021.

#ifndef MAPH_PARSETABLE
#define MAPH_PARSETABLE  1

#include <iostream>
#include <vector>
#include <unordered_map>

#include "itemsets.h"

struct parsetable
{

   struct state
   {
      lr1itemset itemset;

      symbol::map< size_t > shifts;
         // Shifting transitions. Maps symbols to state numbers.

      symbol::map< std::unordered_set< const rule* >> reductions;
         // Maps symbols to the set of rules that can reduce with this 
         // number. 

      bool hasshift( symbol s ) const 
         { return shifts. find(s) != shifts. end( ); } 

      explicit state( const lr1itemset& itemset ) 
         : itemset( itemset ) 
      { } 

      explicit state( lr1itemset&& itemset )
         : itemset( std::move( itemset ))
      { } 

      void print( std::ostream& out, bool showred ) const; 
         // Print the state. If expand is true, then items are expanded
         // before printing. If red is true, then reductions are shown. 
   };

   std::vector< state > states; 

   // We maintain an index that maps lr0itemsets to size_t. 
   // It is used for quick lookup of states. 

   using setmap = 
   std::unordered_map< lr0itemset, size_t, 
                       lr0itemset::hash, lr0itemset::equal_to > ;  

   using iterator = setmap::iterator; 
   using const_iterator = setmap::const_iterator;

   setmap index; 

   symbol::map< size_t > initialstates;
      // Maps possible start symbols to the state in
      // which the parser will start. 

   parsetable( ) = delete; 

   explicit parsetable( const grammar& g );
      // Constructs a basic parser with initial states of form
      // S -> # w for each start symbol S. The constructor
      // does not compute any closures or transitions. 

   void close( const grammar& g, bool keepclosures );
      // The parsetable is initialized as follows:
      // For each start symbol S with lookahead set { L1, ..., Ln },  
      // create a state containing all items of form 
      //    S -> # X / { L1, ..., Ln }, for each rule S -> X. 
      // After that, the complete parsetable is computed.
      //
      // If keepclosures is true, we keep the itemsets in closed form.
      // This has no consequences for the resulting parser, but it may 
      // be useful to see the closures for debugging or teaching. 
      
   size_t nrstates( ) const 
      { return states. size( ); } 

   void selfcheck( ) const;
      // Recheck the parse table and throw a runtime exception
      // if we are unhappy about something.

   void print( std::ostream& out ) const;

};

inline 
std::ostream& operator << ( std::ostream& out, const parsetable tab )
{
   tab. print( out );
   return out;
}
   
#endif


