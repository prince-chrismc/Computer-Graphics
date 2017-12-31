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

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <vector>

namespace Obj
{
   class Model
   {
   friend class Loader; // Model's Builder Implementation
   public:
      Model() = default;
      ~Model() = default;

      std::vector<glm::vec3> GetVertices() const { return m_Vertices; }
      std::vector<glm::vec2> GetTextures() const { return m_Textures; }
      std::vector<glm::vec3> GetNormals() const { return m_Normals; }

   private:
      std::vector<glm::vec3> m_Vertices;
      std::vector<glm::vec2> m_Textures;
      std::vector<glm::vec3> m_Normals;
   };
}

