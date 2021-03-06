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

#include "Alien.h"
#define SINGLETON_SHADER 1
#include "Shader.h"
#include "ObjectColors.h"
#include "Loader.h"
#include "glm/gtc/matrix_transform.hpp"
#include <random>

float Alien::s_AlienScalar = 0.0f;
std::once_flag Alien::Drawable::s_Flag;
std::shared_ptr<Alien::Drawable> Alien::Drawable::s_Instance;

void Alien::Draw(const RenderMode& render_mode) const
{
   auto shaderProgram = Shader::Linked::GetInstance();

   glm::mat4 model_matrix;
   glm::vec3 Alien_scale(BASE_SCALE_FACTOR + s_AlienScalar);
   model_matrix = glm::translate(model_matrix, glm::vec3(m_TransX, m_TransY, 0.0f));
   model_matrix = glm::scale(model_matrix, Alien_scale);
   shaderProgram->SetUniformMat4("model_matrix", model_matrix);

   shaderProgram->SetUniformInt("object_color", (GLint)ObjectColors::BLUE);

   Drawable::GetInstance()->Draw(render_mode);

}

void Alien::MoveTowards(float trans_x, float trans_y)
{
   static std::random_device rd;
   static std::mt19937 rand_gen(rd());

   float dif_x = std::abs(m_TransX - trans_x);
   float dif_y = std::abs(m_TransY - trans_y);

   if(dif_x == 0.0f && dif_y == 0.0f) return;

   if (rand_gen() % 2 == 0)
   {
      if (dif_x >= dif_y)
      {
         (trans_x >= m_TransX) ? m_TransX += STEP_SIZE : m_TransX -= STEP_SIZE;
      }
      else if (dif_y > dif_x)
      {
         (trans_y >= m_TransY) ? m_TransY += STEP_SIZE : m_TransY -= STEP_SIZE;
      }
   }
   else
   {
      if (dif_y < dif_x)
      {
         (trans_y >= m_TransY) ? m_TransY += STEP_SIZE : m_TransY -= STEP_SIZE;
      }
      else if (dif_x >= dif_y)
      {
         (trans_x >= m_TransX) ? m_TransX += STEP_SIZE : m_TransX -= STEP_SIZE;
      }
   }
}

Alien::Drawable::Drawable()
{
   GLuint PositonIndex = Shader::Linked::GetInstance()->GetAttributeLocation("position");

   auto model = Obj::Loader("assets/teapot.obj").GetModel();
   std::vector<glm::vec3> vertices = model.GetVertices();

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

Alien::Drawable::~Drawable()
{
   glDeleteBuffers(1, &m_Vertices);
   //glDeleteBuffers(1, &m_Colors);
   glDeleteVertexArrays(1, &m_VAO);
}

void Alien::Drawable::Draw(const RenderMode& render_mode) const
{
   glBindVertexArray(m_VAO);
   glDrawArrays((GLint)render_mode, 0, m_NumVertices);
   glBindVertexArray(0);
}
