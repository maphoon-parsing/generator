
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

#ifndef MAPH_FILEREADER 
#define MAPH_FILEREADER  1

#include <iostream>
#include <string>
#include <string_view>

class filereader
{
   std::istream* in;
   std::string buffer;

public: 
   std::string filename; 

   long unsigned int line;     // We count like computer scientists, 
   long unsigned int column;   // because that's what we are. 


   filereader( ) :
      in( nullptr ),
      line(0), column(0)
   { }

   filereader( std::istream* in, const std::string& filename ) 
      : in( in ),
        filename( filename ),
        line(0), column(0) 
   { }

   // The moves are transfers of responsibility:

   filereader( filereader&& );
   filereader( const filereader & ) = delete;

   filereader& operator = ( const filereader& ) = delete;
   filereader& operator = ( filereader&& );

   char peek( size_t i ) const;
      // See i-th character in the buffer. It must be in the buffer
      // already, because we won't try to extend the buffer. 
      // This means that has(i') with i' > i must have succeeded before. 

   std::string_view view( size_t i ) const;
      // get a string_view of the initial segment up to, but not 
      // including i.
      // Any non-const operation will spoil the view. 

   bool has( size_t len );
      // True if we have least i characters. If we have less, 
      // we try to read from the file. Therefore we are not const.  
      // After successful has(i), it is guaranteed that we 
      // we have all characters get(i') with i' < i.

   void commit( size_t len ); 
      // irreversably move forward. We must have at least i characters,
      // which means that has(i) must have succeeded before. 

   bool good( ) const { return in -> good( ); } 
   bool bad( ) const { return in -> bad( ); } 
   bool eof( ) const { return in -> eof( ); }

   void print( std::ostream& ) const; 
};

inline std::ostream& operator << ( std::ostream& out, const filereader& read )
{
   read. print( out );
   return out;
}

#endif

