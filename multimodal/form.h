
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


#ifndef FORM_INCLUDED
#define FORM_INCLUDED   1

#include <variant>
#include "agent.h" 
#include "indentation.h"
#include "shared_ptr.h"

enum logop
{
   op_false, op_true,
   op_not,
   op_and, op_or, op_imp, op_equiv,
   op_box, op_dia 
};

const char* getcstring( logop );

inline std::ostream& operator << ( std::ostream& out, logop op )
{
   out << getcstring( op ); return out;
}


struct variable;
struct nullary;
struct unary;
struct binary;
struct modal;

using formbase = std::variant< variable, nullary, unary, binary, modal > ;

struct form 
{
   util::shared_ptr< formbase > pntr;

   form( const form& ) = default; 
   form( form&& ) noexcept = default;

   form& operator = ( const form& ) = default;
   form& operator = ( form&& ) noexcept = default;

   ~form( ) noexcept = default;

   form( const std::string& );
   form( std::string&& );

   form( logop op );

   form( logop op, const form& sub );
   form( logop op, form&& sub );

   form( logop op, const form& sub1, const form& sub2 );
   form( logop op, form&& sub1, form&& sub2 );

   form( logop op, agent a, const form& sub );
   form( logop op, agent a, form&& sub );

   size_t size( ) const; 

   form nnf( int polarity ) const;
      // polarity must be > 0 or < 0. 

   template< typename T > const T* get( ) const;

   void printprefix( std::ostream& out ) const; 
   void printastree( indentation indent, std::ostream& out ) const; 
};


struct variable
{
   std::string name; 
 
   variable( const std::string& name )
      : name( name ) 
   { }
     
   variable( std::string&& name )
      : name( std::move( name ))
   { }  
};


struct nullary
{ 
   logop op;

   nullary( logop op )  
      : op( op )
   { } 
};


struct unary 
{
   logop op; 
   form sub;

   unary( logop op, const form& sub )
      : op( op ), sub( sub )
   { }

   unary( logop op, form&& sub )
      : op( op ), sub( std::move( sub ))
   { }
};

 
struct binary
{
   logop op;
   form sub1; 
   form sub2;

   binary( logop op, const form& sub1, const form& sub2 )
      : op( op ), sub1( sub1 ), sub2( sub2 )
   { }

   binary( logop op, form&& sub1, form&& sub2 )
      : op( op ), sub1( std::move( sub1 )), sub2( std::move( sub2 ))
   { } 
};


struct modal
{
   logop op;
   agent a; 
   form sub;

   modal( logop op, agent a, const form& sub )
      : op( op ), a(a), sub( sub )
   { }

   modal( logop op, agent a, form&& sub )
      : op( op ), a(a), sub( std::move( sub ))
   { }
};

 
template< typename T >
const T* form::get( ) const 
   { return std::get_if<T> ( & *pntr ); }

void printastree( std::ostream& out, indentation ind, const form& f ); 

inline std::ostream& operator << ( std::ostream& out, const form& f )
{
   f. printprefix( out );
   return out;
}

#endif
 
