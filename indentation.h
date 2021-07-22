
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


// Written by Hans de Nivelle, September 2016.

#ifndef MAPH_INDENTATION_
#define MAPH_INDENTATION_  1

#include <iostream>

class indentation
{
   unsigned int s;

public: 
   indentation( ) : s(0) { } 
   explicit indentation( unsigned int s ) : s(s) { }

   indentation operator + ( unsigned int t ) const
      { return indentation{s+t}; }

   indentation operator - ( unsigned int t ) const
      { return indentation{s-t}; }

   indentation& operator ++ ( ) 
      { ++ s; return *this; }
   indentation& operator -- ( ) 
      { -- s; return *this; }

   indentation& operator += ( unsigned int i ) 
      { s += i; return *this; }
   indentation& operator -= ( unsigned int i )
      { s -= i; return *this; }
 
   indentation operator ++ ( int )
      { auto original = *this; ++ s; return original; }
   indentation operator -- ( int )
      { auto original = *this; -- s; return original; } 

   unsigned int size( ) const { return s; } 
      // Expected size when printed.

   void clear( ) { s = 0; } 
};


inline std::ostream& operator << ( std::ostream& out, indentation ind ) 
{
   for( unsigned int i = 0; i < ind. size( ); ++ i )
      out << ' '; 
   return out;
}
 
#endif

