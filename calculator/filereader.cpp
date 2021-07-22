
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


// Written by Hans de Nivelle, 2021.

#include "filereader.h"
#include <utility>

filereader::filereader( filereader&& read )
   : in( std::exchange( read. in, nullptr )),
     filename( std::exchange( read. filename, std::string( ))),
     line( std::exchange( read. line, 0 )), 
     column( std::exchange( read. column, 0 )),
     buffer( std::exchange( read. buffer, std::string( )))
{ }

filereader& filereader::operator = ( filereader&& read )
{
   in = std::exchange( read. in, nullptr ); 
   filename = std::exchange( read. filename, std::string( )); 
   line = std::exchange( read. line, 0 );
   column = std::exchange( read. column, 0 ); 
   buffer = std::exchange( read. buffer, std::string( )); 
   return *this; 
}

char filereader::peek( size_t i ) const 
{
   if( i >= buffer. size( ))
      throw std::runtime_error( "filereader: peek outside of buffer" );
   return buffer[i];
}

std::string_view filereader::view( size_t i ) const
{
   if( i > buffer. size( ))
      throw std::runtime_error( "filereader: view outside of buffer" );

   return std::string_view( buffer.data( ), i );
}

bool filereader::has( size_t len )
{
   if( len > buffer. size( ))
   {
      if( !good( )) 
         return false; 

      while( len > buffer. size( ))
      {
         int c = in -> get( );
         if( !good( ))
            return false;
         buffer. push_back(c);
      }
   }
   return true;  
}

// In a real application, this operation would be unprotected:

void filereader::commit( size_t len ) 
{
   if( len > buffer. size( ))
      throw std::runtime_error( "filereader: commit beyond buffer" ); 

   for( size_t i = 0; i != len; ++ i ) 
   {
      if( buffer[i] == '\n' )
      {
         ++ line;
         column = 0; 
      }
      else
         ++ column;
   }

   buffer. erase( buffer. begin( ), buffer. begin( ) + len ); 
} 

namespace
{
   char hex( char c ) 
   {
      if( c >= 10 )
         return 'A' + ( c - 10 );
      else
         return '0' + c;
   }
}


void filereader::print( std::ostream& out ) const 
{
   if( in == nullptr )
   {
      out << "filereader( nofile )"; 
      return; 
   }

   out << "filereader( " << filename << ", " << line << ", " << 
           column << " ) : ";

   for( char c : buffer )
   {
      if( isprint(c)) 
         out << c; 
      else 
         out << "{" << hex( c >> 4 ) << hex( c & 0X0F ) << "}";
   }
   if( eof( ) ) 
      out << " (end of file)";
   else
   {
      if( !good( ))  
         out << "(file is not good)"; 
   }
   out << "\n";
}


