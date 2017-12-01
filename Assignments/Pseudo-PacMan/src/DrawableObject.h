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

#include <vector>

#include <gl\glew.h>
#include "glm\vec3.hpp"

#include "RenderMode.h"

// This is just as a reference during the rebuild of this application
class DrawableObject abstract
{
   public:
   DrawableObject(const std::vector<glm::vec3> verticies, const std::vector<glm::vec3> colors, const std::vector<GLuint> indicies);
   virtual void Draw(const RenderMode& render_mode);
   virtual void Delete();

   protected:
      GLuint m_VAO;
      GLuint m_Verticies;
      GLuint m_Colors;
      GLuint m_Indicies;

      GLsizei m_NumVertices;
      GLsizei m_NumIndicies;
};