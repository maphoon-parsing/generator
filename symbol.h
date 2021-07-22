
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


// Written by Hans de Nivelle, January 2021.

#ifndef MAPH_SYMBOL
#define MAPH_SYMBOL   1

#include <string>
#include <iostream>
#include <unordered_set> 
#include <unordered_map>

class symbol 
{
   using indextype = std::unordered_set< std::string > ; 
   indextype::const_iterator p; 

public: 
   symbol( const std::string& s )
      : p( getindex( ). insert(s). first ) 
   { }

   symbol( const char* c ) 
      : symbol( std::string(c) )
   { }

   const std::string& getstring( ) const
      { return *p; } 

   static void printtable( std::ostream& stream );
      // Prints table of all symbols. 

   static symbol recover( ); 
      // Recover can be used for setting recovery points in 
      // rules. Like in Expression -> RECOVER dot.
      // Maphoon will try to recover on a dot after a syntax error.

   static indextype& getindex( ) 
   {
      static indextype ind;
      return ind;
   }

   bool operator == ( symbol s ) const { return p == s.p; } 
   bool operator != ( symbol s ) const { return p != s.p; } 

   struct hash
   {
      size_t operator( ) ( symbol s ) const
         { return reinterpret_cast<size_t> ( & * s.p ); }
   };

   struct equal_to
   {
      bool operator( ) ( symbol s1, symbol s2 ) const
         { return s1 == s2; }
   };

   template< typename D > using map =
   std::unordered_map< symbol, D, hash, equal_to > ;
};

inline std::ostream& operator << ( std::ostream& out, symbol s )
{
   out << s. getstring( ); 
   return out;
}

template< typename D > std::ostream&
operator << ( std::ostream& stream, const symbol::map<D> & mp )
{
   for( const auto& p : mp )
   {
      stream << "   " << p. first << " : " << p. second << "\n";
   }

   return stream;
}

#endif


