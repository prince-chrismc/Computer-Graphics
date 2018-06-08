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

#include "BuilderUtility.h"
#include "glm\vec3.hpp"
#include <string>

class Light
{
   public:
      Light(const glm::vec3& pos, const glm::vec3& col) : m_Pos(pos), m_Col(col) {}
      Light() : Light(glm::vec3(), glm::vec3()) {}

       glm::vec3 GetPosition() const { return m_Pos; }
       glm::vec3 GetColor() const { return m_Col; }

      class Builder : private BuilderUtility
      {
         public:
            Builder() = default;
            Builder(const Builder&) = delete;
            void operator=(const Builder&) = delete;

            const Builder& ParseLight(const std::string& data);
            Light GetLight() const { return Light(m_Pos, m_Col); }

         private:
            glm::vec3 m_Pos;
            glm::vec3 m_Col;

            const char* COL = "col:";
      };

   private:
      glm::vec3 m_Pos;
      glm::vec3 m_Col;
};
