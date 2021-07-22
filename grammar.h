
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


// Code written by Hans de Nivelle, november 2006.
// Rewritten July 2008. (Added the possibility of multiple start 
// symbols)

#ifndef MAPH_GRAMMAR
#define MAPH_GRAMMAR  1

#include <vector>
#include <iostream>
#include <unordered_map>

#include "rule.h"
#include "symbolset.h"

struct grammar 
{
   symbol::map< symbolset > startsymbols; 
      // Contains the start symbols with their lookahead sets.
      // No symbol in a lookahead set should be reachable from its
      // start symbol.

   std::vector< rule > rules;

   symbol::map< std::unordered_set< size_t >> index;
      // Maps symbols to the indices of the rules that have
      // have this symbol as lhs.

   grammar( ) = default;

   void push_back( rule&& r ); 
   void push_back( const rule& r ); 
      // Append a rule to the grammar. 
      // We don't check for uniqueness of rules. 

   void addstartsym( symbol start, const symbolset& lookahead );
      // A lookahead symbol must not be reachable from the
      // start symbol. We don't check this. It has to be checked
      // later. 

   symbolset leftsymbols( ) const;
      // Collect the set of symbols that occur in the left hand side
      // of a rule. 

   symbolset rightsymbols( ) const;
      // Collect the set of symbols that occur in the right hand side
      // of a rule. 

   symbolset allsymbols( ) const;
      // Collect all symbols of the grammar.

   symbolset nullablesymbols( ) const;
      // Compute the set of symbols that are nullable. These
      // are the symbols that can be rewritten into the empty word. 
      // N is the smallest set satisfying: 
      // If there is a rule A -> B1 ... Bk such that all B1,B2,..,Bk are in
      // N, then A in N

   symbolset reachablesymbols( symbol s ) const; 
      // The set of symbols that are reachable from s. 

   bool hasrule( symbol s ) const; 
      // True if there is a rule that has s as left hand side symbol. 

   symbol::map< symbolset > 
   firstsets( const symbolset& allsymbols, const symbolset& nullable ) const;
      // We need allsymbols, because every symbol is in its own firstset.

   size_t nrrules( ) const 
      { return rules. size( ); }

   static grammar example1( ); 
      // A simple grammar with expressions and operators.

   static grammar example3( ); 

};

std::ostream& operator << ( std::ostream& stream, const grammar& g );

#endif


