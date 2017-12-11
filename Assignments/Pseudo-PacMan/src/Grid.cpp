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


#include "Grid.h"
#include "Shader.h"
#include "ObjectColors.h"
#include "glm\vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

Grid::Grid(const unsigned int& grid_size)
{
   auto shaderProgram = ShaderLinker::GetInstance();
   GLuint PositonIndex = shaderProgram->GetAttributeLocation("position");

   std::vector<glm::vec3> vertices;

   float half_grid(grid_size / 2.0f);
   float half_length(half_grid + 0.5f);

   for (unsigned int index = 0; index <= grid_size; index++)
   {
      // line of x-axis
      vertices.emplace_back(float(index - half_grid), 0 - half_length, 0.0f);
      vertices.emplace_back(float(index - half_grid), half_length, 0.0f);

      // line of y-axis
      vertices.emplace_back(0 - half_length, float(index - half_grid), 0.0f);
      vertices.emplace_back(half_length, float(index - half_grid), 0.0f);
   }

   glGenVertexArrays(1, &m_VAO);
   glBindVertexArray(m_VAO);

   glGenBuffers(1, &m_Vertices);
   glBindBuffer(GL_ARRAY_BUFFER, m_Vertices);
   glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
   glVertexAttribPointer(PositonIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(PositonIndex);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glBindVertexArray(0);

   m_NumVertices = (GLsizei)vertices.size();
}

Grid::~Grid()
{
   glDeleteBuffers(1, &m_Vertices);
   glDeleteVertexArrays(1, &m_VAO);
}

void Grid::Draw()
{
   auto shaderProgram = ShaderLinker::GetInstance();
   shaderProgram->SetUniformInt("object_color", (GLint)ObjectColors::GREY);
   shaderProgram->SetUniformMat4("model_matrix", glm::scale(glm::mat4(), glm::vec3(0.25f)));

   glBindVertexArray(m_VAO);
   glDrawArrays(GL_LINES, 0, m_NumVertices);
   glBindVertexArray(0);
}
