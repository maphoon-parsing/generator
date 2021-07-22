
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


#include "filereader.h"
#include "symbol.h" 
#include "tokenizing.h"
#include "parser.h"

void testtokenizer( filereader& read )
{

   symbol sym = tokenizing::readsymbol( read );
   while( sym. type != sym_EOF )
   {
      std::cout << sym << "\n";
      sym = tokenizing::readsymbol( read );
   }
   std::cout << "that's end of file\n";

}


int main( int argc, char* argv [] )
{
   varstore<double> vs;
   vs. assign( "e", 2.71828182845904523536 );
   vs. assign( "pi", 3.14159265358979323846264338327950288 ); 

   std::cout << vs << "\n";

   filereader read( &std::cin, "stdin" );
   // testtokenizer( read );

   std::vector< std::string > errorlog;

   parser prs( vs, errorlog, read );
 
   prs. debug = 2;  
   auto sym = prs. parse( sym_Session );
   std::cout << "parser returned " << sym << "\n";
   return 0;
}

void parser::merge_infos( location& loc1, location&& loc2 ) 
{ 
   std::cout << "merging info " << loc1 << " " << loc2 << "\n";
} 

