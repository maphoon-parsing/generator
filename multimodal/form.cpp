
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


#include "form.h"


const char* getcstring( logop op )
{
   switch( op )
   {
   case op_false: return "FALSE";
   case op_true: return "TRUE";
   case op_not: return "~"; 
   case op_and: return "&";
   case op_or: return "|";
   case op_imp: return "->";
   case op_equiv: return "<->";
   case op_box: return "[]";
   case op_dia: return "<>"; 
   }
   return "???";
}


form::form( const std::string& name )
   : pntr( new formbase( name ))
{ }

form::form( std::string&& name )
   : pntr( new formbase( std::in_place_type_t< variable > ( ),
                                 std::move( name )))
{ }

form:: form( logop op )
   : pntr( new formbase( std::in_place_type_t< nullary > ( ), op ))
{ }

form::form( logop op, const form& sub )
   : pntr( new formbase( std::in_place_type_t< unary > ( ), op, sub ))
{ }

form::form( logop op, form&& sub )
   : pntr( new formbase( std::in_place_type_t< unary > ( ), 
                         op, std::move( sub )))
{ }

form::form( logop op, const form& sub1, const form& sub2 )
   : pntr( new formbase( std::in_place_type_t< binary > ( ), 
                         op, sub1, sub2 ))
{ }

form::form( logop op, form&& sub1, form&& sub2 )
   : pntr( new formbase( std::in_place_type_t< binary > ( ), 
                         op, std::move(sub1), std::move(sub2) ))
{ }
        
form::form( logop op, agent a, const form& sub )
   : pntr( new formbase( std::in_place_type_t< modal > ( ),
                         op, a, sub ))
{ }

form::form( logop op, agent a, form&& sub )
   : pntr( new formbase( std::in_place_type_t< modal > ( ),
                         op, a, std::move( sub )))
{ }



void form::printprefix( std::ostream& out ) const
{
   if( auto v = get<variable> ( ))
   {
      out << ( v -> name );
      return;
   }

   if( auto n = get<nullary> ( ))
   {
      out << ( n -> op ); 
      return;
   }

   if( auto u = get<unary> ( ))
   {
      out << ( u -> op ) << "( ";
      u -> sub. printprefix( out );
      out << " )";
      return;
   }
   
   if( auto b = get<binary> ())
   {
      out << ( b -> op ) << "( ";
      b -> sub1. printprefix( out );
      out << ", ";
      b -> sub2. printprefix( out );
      out << " )";
      return;
   }

   if( auto m = get<modal> ())
   {
      out << getcstring( m -> op )[0] << ' ';
      out << ( m -> a ) << ' ' << getcstring( m -> op )[1] << "( ";
      m -> sub. printprefix( out );
      out << " )"; 
      return;
   }

   throw std::logic_error( "dont know" ); 
}


form form::nnf( int polarity ) const
{
   if( auto v = get<variable> ( ))
   {
      if( polarity > 0 ) 
         return *this;
      else
         return form( op_not, *this ); 
   }

   if( auto n = get<nullary> ( ))
   {
      if( n -> op == op_false )
         return polarity > 0 ? form( op_false ) : form( op_true );  
      else
         return polarity > 0 ? form( op_true ) : form( op_false ); 
   }

   if( auto u = get<unary> ( ))
   {
      return u -> sub. nnf( -polarity );
   }

    
   if( auto b = get<binary> ())
   {
      switch( b -> op ) 
      {
      case op_and: 
         if( polarity > 0 )
            return form( op_and, b -> sub1. nnf(1),
                                 b -> sub2. nnf(1)); 
         else
            return form( op_or, b -> sub1. nnf(-1), 
                                b -> sub2. nnf(-1)); 
      case op_or:
         if( polarity > 0 )
            return form( op_or, b -> sub1. nnf(1), 
                                b -> sub2. nnf(1));
         else
            return form( op_and, b -> sub1. nnf(-1),
                                 b -> sub2. nnf(-1)); 
      case op_imp:
         if( polarity > 0 )
            return form( op_or, b -> sub1. nnf(-1), 
                                b -> sub2. nnf(1));
         else
            return form( op_and, b -> sub1. nnf(1), 
                                 b -> sub2. nnf(-1 ));

      case op_equiv:
         if( polarity > 0 )
            return form( op_and, 
                       form( op_or, b -> sub1. nnf(-1),
                                    b -> sub2. nnf(1) ),
                       form( op_or, b -> sub1. nnf(1),
                                    b -> sub2. nnf(-1 ))); 
         else
            return form( op_or,
                      form( op_and, b -> sub1. nnf(-1),
                                    b -> sub2. nnf(1) ),
                      form( op_and, b -> sub1. nnf(1),
                                    b -> sub2. nnf(-1) )); 
      }
   }

   if( auto m = get<modal> ())
   {
      if( polarity > 0 )
         return form( m -> op, m -> a, m -> sub. nnf(1));
      else
         return form( m -> op == op_box ? op_dia : op_box, 
                      m -> a,
                      m -> sub. nnf(-1));
   } 
   throw std::logic_error( "dont know" ); 
}

