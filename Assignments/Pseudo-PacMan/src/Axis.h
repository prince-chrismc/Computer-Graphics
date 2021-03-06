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
#include "ObjectColors.h"
#include "glm\vec3.hpp"
#include <gl\glew.h>
#include <initializer_list>

class Line
{
public:
   Line(std::initializer_list<glm::vec3> pts, const ObjectColors& col);
   ~Line();

   void Draw() const;

   private:
      GLuint m_VAO;
      GLuint m_Vertices;

      GLsizei m_NumVertices;

      const ObjectColors m_Color;
};

class XYZ_Axis
{
   public:
      XYZ_Axis() : m_X({ { -0.5f, 0.0f, 0.0f },{ 2.5f, 0.0f, 0.0f } }, ObjectColors::RED), m_Y({ { 0.0f, -0.5f, 0.0f },{ 0.0f, 2.5f, 0.0f } }, ObjectColors::GREEN),
                   m_Z({ { 0.0f, 0.0f, -0.5f },{ 0.0f, 0.0f, 2.5f } }, ObjectColors::BLUE) {}

      void Draw() const { m_X.Draw(); m_Y.Draw(); m_Z.Draw(); }

   private:
      Line m_X;
      Line m_Y;
      Line m_Z;
};
