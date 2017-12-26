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

#include "Shaders.h"
#include <iostream>

bool Shader::Vertex::Compile(const std::string& glsl_code)
{
   m_Id = glCreateShader(GL_VERTEX_SHADER);
   char const* glsl_buffer = glsl_code.c_str();
   glShaderSource(m_Id, 1, &glsl_buffer, NULL);
   glCompileShader(m_Id);

   // Check for compile time errors
   GLint success;
   GLchar info_log[512];
   glGetShaderiv(m_Id, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      glGetShaderInfoLog(m_Id, 512, NULL, info_log);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
      return false;
   }
   return true;
}

bool Shader::Fragment::Compile(const std::string & glsl_code)
{
   m_Id = glCreateShader(GL_FRAGMENT_SHADER);
   char const* glsl_buffer = glsl_code.c_str();
   glShaderSource(m_Id, 1, &glsl_buffer, NULL);
   glCompileShader(m_Id);

   // Check for compile time errors
   GLint success;
   GLchar info_log[512];
   glGetShaderiv(m_Id, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      glGetShaderInfoLog(m_Id, 512, NULL, info_log);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
      return false;
   }
   return true;
}
