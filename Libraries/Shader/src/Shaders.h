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

#pragma once

#include "Interface.h"

namespace Shader
{
   class Vertex : public IShader
   {
   public:
      Vertex(const std::string& rel_path) : IShader(rel_path) { m_Status = Compile(m_Code); }
      Vertex(const char* rel_path) : Vertex(std::string(rel_path)) {}

      bool Compile(const std::string& glsl_code);
   };

   class Fragment : public IShader
   {
   public:
      Fragment(const std::string& rel_path) : IShader(rel_path) { m_Status = Compile(m_Code); }
      Fragment(const char* rel_path) : Fragment(std::string(rel_path)) {}

      bool Compile(const std::string& glsl_code);
   };
}

