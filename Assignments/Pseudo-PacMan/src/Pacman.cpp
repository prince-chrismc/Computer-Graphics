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

#include "Pacman.h"
#include "Shader.h"
#include "ObjectColors.h"
#include "objloader.h"
#include "glm/gtc/matrix_transform.hpp"
#include <random>

float Pacman::s_PacmanScalar = 0.0f;
std::once_flag Pacman::Drawable::s_Flag;
std::shared_ptr<Pacman::Drawable> Pacman::Drawable::s_Instance;

void Pacman::Draw(const RenderMode& render_mode) const
{
   auto shaderProgram = ShaderLinker::GetInstance();

   glm::mat4 model_matrix;
   glm::vec3 Pacman_scale(BASE_SCALE_FACTOR + s_PacmanScalar);
   model_matrix = glm::translate(model_matrix, glm::vec3(m_TransX, m_TransY, 0.0f));
   model_matrix = glm::rotate(model_matrix, glm::radians((float)m_Direction), glm::vec3(0.0f, 0.0f, 1.0f));
   model_matrix = glm::scale(model_matrix, Pacman_scale);
   shaderProgram->SetUniformMat4("model_matrix", model_matrix);

   shaderProgram->SetUniformInt("object_color", (GLint)ObjectColors::YELLOW);

   Drawable::GetInstance()->Draw(render_mode);

}

void Pacman::Move(const Direction& dir, const unsigned int& grid_size)
{
   float upper_move_limit = float((grid_size / 2.0)*0.25);
   float lower_move_limit = float(0 - ((grid_size / 2.0)*0.25));

   switch (dir)
   {
   case Direction::D_KEY:
      if (m_TransX < upper_move_limit)
         m_TransX += STEP_SIZE;
      break;
   case Direction::W_KEY:
      if (m_TransY < upper_move_limit)
         m_TransY += STEP_SIZE;
      break;
   case Direction::A_KEY:
      if (m_TransX > lower_move_limit)
         m_TransX -= STEP_SIZE;
      break;
   case Direction::S_KEY:
      if (m_TransY > lower_move_limit)
         m_TransY -= STEP_SIZE;
      break;
   default:
      break;
   }

   m_Direction = dir;
}

Pacman::Drawable::Drawable()
{
   GLuint PositonIndex = ShaderLinker::GetInstance()->GetAttributeLocation("position");

   std::vector<glm::vec3> vertices;
   LoadObjFile("assets/pacman.obj", &vertices, &std::vector<glm::vec3>(), &std::vector<glm::vec2>()); // parse the .obj file for the vertices only

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

Pacman::Drawable::~Drawable()
{
   glDeleteBuffers(1, &m_Vertices);
   //glDeleteBuffers(1, &m_Colors);
   glDeleteVertexArrays(1, &m_VAO);
}

void Pacman::Drawable::Draw(const RenderMode& render_mode) const
{
   glBindVertexArray(m_VAO);
   glDrawArrays((GLint)render_mode, 0, m_NumVertices);
   glBindVertexArray(0);
}
