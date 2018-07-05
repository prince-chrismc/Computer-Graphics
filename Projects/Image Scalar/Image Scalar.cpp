/*
MIT License

Copyright (c) 2018 Chris McArthur, prince.chrismc(at)gmail(dot)com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// Image Scalar.cpp : Defines the entry point for the console application.
//

#include "CmdLineParser.h"

#define cimg_use_png 1
#include "CImg.h"

#include <iostream>

void PrintUsage( const std::string& app_name );

int main( int argc, char** argv )
{
   CommandLineParser oParser( argc, argv );

   if( !oParser.DoesSwitchExists( "f" ) || !oParser.DoesSwitchExists( "file" ) )
   {
      PrintUsage( oParser.GetProgramName() );
      return -1;
   }

   cimg_library::CImg<double> oImage;
   oImage.load_png( oParser.GetPairValue( "-f" ).c_str() );

   return 0;
}

void PrintUsage( const std::string& app_name )
{
   std::cout << app_name << "-file=<file_name>" << std::endl;
   std::cout << " -file | -f : Relative path or absolute path to the file." << std::endl;
}
