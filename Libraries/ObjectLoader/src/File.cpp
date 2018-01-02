/*
MIT License

Copyright (c) 2017 Chris McArthur, prince.chrismc(at)gmail(dot)com

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

#include "File.h"
#include <fstream>
#include <iostream>

Obj::File::File(const char* file_path)
{
   std::ifstream obj_file(file_path, std::ios::in);

   if (!obj_file.is_open())
   {
      std::cout << "Unable to open file: " << file_path << std::endl;
      return;
   }

   std::string line_buffer = "";
   while (std::getline(obj_file, line_buffer))
   {
      if(line_buffer.empty()) continue;                     // lets not store empty lines
      if(line_buffer.find_first_of("#") == 0) continue;     // ignore comments
      if(line_buffer.length() < 2) continue;                // these shouldnt exist

      m_Lines.push_back(line_buffer);
   }

   obj_file.close();
}
