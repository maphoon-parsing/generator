
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


// Written by Hans de Nivelle, March 2021.

// These are the same tables that go into the code of the parser. 
// This code eventually has to be copied to idee.x. 
// In the parser, symbol must be replaced by symboltype.

#ifndef MAPH_TABLECODEBUILDER
#define MAPH_TABLECODEBUILDER   1

#include "symbol.h"
#include "symbolset.h"
#include "parsetable.h"
#include "cpp.h"

#include <vector>


struct tablecodebuilder
{

   struct statesymbol
   {
      size_t state;
      symbol sym;

      statesymbol( size_t state, symbol sym )
         : state( state ), sym( sym )
      { }

      struct hash
      {
         size_t operator( ) ( const statesymbol& ) const;
      };

      struct equal_to
      {
         bool operator( ) ( const statesymbol &, const statesymbol& ) const;
      };

      template< typename D > using map =
      std::unordered_map< statesymbol, D, hash, equal_to > ;
   };


   symbol::map< std::pair< size_t, symbolset >> startsymbols;
      // Maps possible start symbols to pairs consisting of
      // initial state, and their associated terminator sets.

   std::vector< std::pair< char, size_t >> reductioninfo;
      // Indexed by the states. 
      // char = 'T' : the state consists of a single reduction (trivial). 
      // char = 'D' : the state has a default reduction, which will be
      //              carried out when lookahead has no shift. 
      // char = 'N' : the state has no reductions.
      // char = 'H' : none of the above. The state is hard. 

   statesymbol::map< size_t > shifts; 
   statesymbol::map< std::vector< size_t >> reductions;

      // We use different tables for shifts and for reductions.
      // This is not inefficient, because most reductions will be handled
      // by defaults, which will be not in the reduction table. 
      // If you would store reductions in the same table as shift,
      // you would not another boolean or whatever to store the fact that 
      // there are no reductions. 
      // I hope that in this way, reductions will be looked at very rarely.

   tablecodebuilder( ) = default;

   void 
   printinspace( const cpp::nspace& space, 
                 const char* c, std::ostream& out ) const;

   void 
   print( const cpp::nspace& space, symbol s, std::ostream& out ) const;

   void
   print( const cpp::nspace& space, statesymbol ss, std::ostream& out ) const;

   void printascode( std::ostream& out, 
                     const cpp::nspace& parserspace,
                     const cpp::nspace& symbolspace ) const;
      // Print them as code that can be included in parser.cpp.
      // prefix is prepended to every token. If you want them to be
      // in a namespace, you have to put it in prefix. 

};

#endif

