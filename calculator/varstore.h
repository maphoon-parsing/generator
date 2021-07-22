
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


#ifndef VARSTORE_INCLUDED
#define VARSTORE_INCLUDED   1

#include <map>
#include <string>
#include <iostream>

template< typename D > 
class varstore
{

   std::map<std::string,D> table;

public:
   varstore( ) = default; 

   bool contains( const std::string& var ) const 
      { return table. find( var ) != table. end( ); }

   void assign( const std::string& var, const D& val )
   { 
      auto p = table. insert( std::make_pair( var, val ));
      if( !p. second ) p. first -> second = val;
   }

   const D* lookup( const std::string& var ) const
   {
      auto p = table. find( var );
      if( p != table. end( ))
         return & ( p -> second );
      else
         return nullptr;
   }

   D* lookup( const std::string& var )
   {
      auto p = table. find( var );
      if( p != table. end( ))
         return & ( p -> second );
      else
         return nullptr;
   }

   void clear( ) 
      { table. clear( ); } 

   void print( std::ostream& out ) const
   {
      out << "Variable Store:\n";
      for( const auto& t : table )
         out << "   " << t. first << " = " << t. second << "\n";
      out << "\n";
   }

};

template< typename D > 
inline std::ostream& operator << ( std::ostream& out, const varstore<D>& vs )
{
   vs. print( out );
   return out;
}

#endif

