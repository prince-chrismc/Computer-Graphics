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

#include "Interfaces.h"
#include <iostream>
#include <fstream>

Shader::IShader::IShader(const std::string& rel_path) : m_Status(false)
{
   std::ifstream glsl_file(rel_path, std::ios::in);

   if (glsl_file.is_open())
   {
      std::string line_buffer = "";
      while (std::getline(glsl_file, line_buffer))
      {
         m_Code += "\n" + line_buffer;
      }
      glsl_file.close();
      m_Status = true;
   }
   else
   {
      std::cout << "Impossible to open " << rel_path.c_str() << ". Are you in the right directory ?" << std::endl;
      std::getline(std::cin, std::string());
   }
}

void Shader::IProgram::Link(IShader* vertex, IShader* frag)
{
   if (vertex->m_Status)
   {
      AddShader(vertex);
      m_Status = true;
   }

   if (frag->m_Status && m_Status)
   {
      AddShader(frag);
   }

   // Now lets link the program
   if (m_Status)
   {
      glLinkProgram(m_ProgramId);

      // Check for linking errors
      GLint success;
      GLchar log_buffer[512];
      glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &success);
      if (!success)
      {
         glGetProgramInfoLog(m_ProgramId, 512, NULL, log_buffer);
         std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << log_buffer << std::endl;
         m_Status = false;
      }
   }
}
