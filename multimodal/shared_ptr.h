
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


// Written by Hans de Nivelle, 17 march 2021.
// Revised 31 march 2021.
// A better shared_ptr. I dislike std::shared_ptr.

// The current (5th?) implementation allocates its counter only
// when it shares. I assume that most pointers 
// don't share.
// Unfortunately, the copy constructor can throw
// and becomes a bit more complicated. Also,
// the counter field must be mutable. 

#ifndef UTIL_SHARED_PTR_INCLUDED
#define UTIL_SHARED_PTR_INCLUDED  1

#include <cstddef>
#include <utility>

namespace util
{

   template< typename D > 
   class shared_ptr
   {
      D* p;
      mutable size_t* counter;

   public: 
      shared_ptr( ) noexcept
         : p( nullptr ),
           counter( nullptr )
      { }

      shared_ptr( D* p ) noexcept
         : p(p),
           counter( nullptr ) 
      { }

      shared_ptr( shared_ptr&& sh ) noexcept
         : p( std::exchange( sh.p, nullptr )),
           counter( std::exchange( sh. counter, nullptr ))
      { }


      // I assume that if the new throws, the destructor
      // will not called.

      shared_ptr( const shared_ptr& sh )
         : p( sh.p ),
           counter( sh. counter ? sh. counter : new size_t(2)) 
      {
         if( sh. counter )
            ++ *counter;
         else
            sh. counter = counter;
      }

      shared_ptr& operator = ( shared_ptr&& sh ) noexcept
      {
         std::swap( p, sh. p );
         std::swap( counter, sh. counter );
         return *this; 
      }

      shared_ptr& operator = ( const shared_ptr& sh ) 
      {
         *this = shared_ptr( sh );
         return *this; 
      }

      ~shared_ptr( ) noexcept
      {
         if( counter )
         {
            if( ! -- * counter )
            {
               delete counter; 
               delete p; 
            }
         }
         else
            delete p; 
      }

      // Completely unprotected:

      D& operator * ( ) 
      {   
         return *p;
      }

      const D& operator * ( ) const
      {
         return *p;
      }

      D* operator -> ( ) 
      {
         return p;
      }

      const D* operator -> ( ) const
      {
         return p;
      }

      operator bool( ) const
      {
         return p != nullptr;
      }

      template< typename C > 
      const C* try_cast( ) const 
         { return dynamic_cast< const C* > (p); } 

      template< typename C >
      C* try_cast( ) 
         { return dynamic_cast< C* > (p); } 

   };

}

#endif


