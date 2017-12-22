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

#include "Axis.h"
#include "Shader.h"
#include "RenderMode.h"
#include "glm/gtc/matrix_transform.hpp"

Line::Line(std::initializer_list<glm::vec3> pts, const ObjectColors& col) : m_Color(col)
{
   if (pts.size() >= 2)
   {
      GLuint PositonIndex = ShaderLinker::GetInstance()->GetAttributeLocation("position");

      glGenVertexArrays(1, &m_VAO);
      glBindVertexArray(m_VAO);

      glGenBuffers(1, &m_Vertices);
      glBindBuffer(GL_ARRAY_BUFFER, m_Vertices);
      glBufferData(GL_ARRAY_BUFFER, pts.size() * sizeof(glm::vec3), pts.begin(), GL_STATIC_DRAW);
      glVertexAttribPointer(PositonIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
      glEnableVertexAttribArray(PositonIndex);
      glBindBuffer(GL_ARRAY_BUFFER, 0);

      glBindVertexArray(0);

      m_NumVertices = (GLsizei)pts.size();
   }
}

Line::~Line()
{
   glDeleteBuffers(1, &m_Vertices);
   glDeleteVertexArrays(1, &m_VAO);
}

void Line::Draw() const
{
   auto shaderProgram = ShaderLinker::GetInstance();
   shaderProgram->SetUniformInt("object_color", (GLint)m_Color);
   shaderProgram->SetUniformMat4("model_matrix", glm::scale(glm::mat4(), glm::vec3(0.25f)));

   glBindVertexArray(m_VAO);
   glDrawArrays((GLint)RenderMode::LINES, 0, m_NumVertices);
   glBindVertexArray(0);
}
