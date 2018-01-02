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

#include "GL/glew.h"                            // include GL Extension Wrangler
#include "glm/gtc/type_ptr.hpp"                 // glm::value_ptr
#include <string>

namespace Shader
{
   class IShader abstract
   {
      friend class IProgram;
      public:
         IShader(const std::string& rel_path);
         ~IShader() { glDeleteShader(m_Id); }

         bool operator()() const { return m_Status; }

      protected:
         virtual void Compile() = 0;

         GLuint m_Id;
         bool m_Status;
         std::string m_Code;
   };

   class IProgram abstract
   {
      public:
         IProgram() : m_Status(false), m_ShaderCounter(0) { m_ProgramId = glCreateProgram(); }
         ~IProgram() { glDeleteProgram(m_ProgramId); }

         bool Link(IShader* vertex, IShader* frag);
         void Activate() const { glUseProgram(m_ProgramId); }

         bool operator()() const { return m_Status; }

         virtual GLuint GetUniformLocation(const char* shader_obj) const { return glGetUniformLocation(m_ProgramId, shader_obj); }
         virtual GLuint GetAttributeLocation(const char* shader_obj) const { return glGetAttribLocation(m_ProgramId, shader_obj); }

         void SetUniformInt(const char* shader_obj, const GLint& i) const { glUniform1i(GetUniformLocation(shader_obj), i); }
         void SetUniformMat4(const char* shader_obj, const glm::mat4& mat) const { glUniformMatrix4fv(GetUniformLocation(shader_obj), 1, GL_FALSE, glm::value_ptr(mat)); }
         void SetUniformVec3(const char* shader_obj, const glm::vec3& vec) const { glUniform3fv(GetUniformLocation(shader_obj), 1, glm::value_ptr(vec)); }

      private:
         GLuint m_ProgramId;
         bool m_Status;
         unsigned int m_ShaderCounter;

         bool AddShader(IShader* shader);
   };
}

