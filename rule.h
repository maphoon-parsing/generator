
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


// Written by Hans de Nivelle, October 2005. 
// A rule is a thing that looks like
// A -> B1 ... Bn, in which A, B1, ..., Bn symbols and n >= 0. 
// 
// In June 2008, I removed the priority and associativity from rules,
// because I think that priorities are a thing between symbols.
//
// Rewritten in February 2021. 

#ifndef MAPH_RULE
#define MAPH_RULE  1

#include <vector>
#include <iostream>

#include "symbol.h"

struct rule 
{
   size_t nr;

   symbol lhs;
   std::vector< symbol > rhs;

   rule( size_t nr, symbol lhs, const std::vector< symbol > & rhs )
      : nr( nr ), lhs( lhs ), rhs( rhs )
   { }

   rule( size_t nr, symbol lhs, std::vector< symbol > && rhs )
      : nr( nr ), lhs( lhs ), rhs( std::move( rhs ))
   { }

   using rhs_iterator = std::vector< symbol > :: const_iterator;
      // The dot in an item. 
 
   rhs_iterator rhs_begin( ) const { return rhs. begin( ); }
   rhs_iterator rhs_end( ) const { return rhs. end( ); }
};

std::ostream& operator << ( std::ostream& , const rule& );

#endif


