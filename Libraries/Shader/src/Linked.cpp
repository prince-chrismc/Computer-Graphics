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

#include "Linked.h"
#include "Shader.h"
#include <iostream>

std::once_flag Shader::Linked::s_Flag;
std::shared_ptr<Shader::Linked> Shader::Linked::s_Instance;

bool Shader::Linked::Link(IShader* vertex, IShader* frag)
{
   AddShader(vertex);
   AddShader(frag);

   glLinkProgram(m_ShaderProgram);

   // Check for linking errors
   GLint success;
   GLchar infoLog[512];
   glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
   if (!success) {
      glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
      return false;
   }

   glUseProgram(m_ShaderProgram);
   return true;
}

void Shader::Linked::AddShader(IShader* shader)
{
   glAttachShader(m_ShaderProgram, shader->m_Shader);
}
