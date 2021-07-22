
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


// this file defines syntaxerror and readerror. A readerror 
// means that reading of input was attempted and failed, for some
// reason different than encountering end of file.
//
// A syntaxerror means that it was attempted to read a term, 
// that enough characters were successfully read, but that they could not
// be parsed according to the input grammar.
//
// Code written by Hans de Nivelle, April 2006.

#ifndef MAPH_ERRORS
#define MAPH_ERRORS 1 

#include <string>
#include <iostream>

#include "itemsets.h"

struct syntaxerror
{
   std::string problem;
      
   std::string filename;
   long unsigned int line;
   long unsigned int column;

   syntaxerror( const std::string& problem,
		const std::string& filename,
		long unsigned int line,
                long unsigned int column ) 
      : problem( problem ),
        filename( filename ),
	line( line ),
        column( column ) 
   { }

};

std::ostream& operator << ( std::ostream&, const syntaxerror& ); 


struct generalerror
{
   std::string problem;

   std::string filename;
   long unsigned int line;
   long unsigned int column; 

   // The description of the problem should be such that in can
   // be followed by 'in line ... of file ...'.

   generalerror( const std::string& problem,
                 const std::string& filename,
                 long unsigned int line,
                 long unsigned int column ) 
      : problem( problem ),
        filename( filename ),
	line( line ),
        column( column ) 
   { }
};

std::ostream& operator << ( std::ostream&, const generalerror& );


struct conflict 
{
   size_t statenr;
   symbol::map< std::vector< size_t >> badrules;
      // Maps lookahead symbols to the numbers of the problematic rules. 
      // If the left hand sides are not included in one of the reduction 
      // sequences, then these are all rules for which a reduction is 
      // possible. 
      // Otherwise, these are the rules that are not last, and that
      // have no condition (requires) attached. 

   explicit conflict( size_t statenr )
      : statenr( statenr )
   { } 

   bool empty( ) const 
      { return badrules. size( ) == 0; } 

};

std::ostream& operator << ( std::ostream&, const conflict& );


struct ideeproblem 
{
   std::string problem; 
   long unsigned int line;
  
   ideeproblem( const std::string& problem, long unsigned int line )
      : problem( problem ), line( line )
   { }
};

std::ostream& operator << ( std::ostream&, const ideeproblem& ); 

#endif


