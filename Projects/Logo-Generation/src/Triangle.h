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
#include "SceneElement.h"
#include <string>

class Triangle : public SceneElement
{
   public:
      Triangle() = default;
      Triangle(const glm::vec3& vert1, const glm::vec3& vert2, const glm::vec3& vert3, const glm::vec3& amb, const glm::vec3& dif, const glm::vec3& spe, const float& shine) :
         m_Vert1(vert1), m_Vert2(vert2), m_Vert3(vert3), m_Amb(amb), m_Dif(dif), m_Spe(spe), m_Shine(shine) {}

      bool TestIntersection(const glm::vec3& cam_pos, const glm::vec3& ray_dir, glm::vec3* out_intersection, float* out_distance) const;
      glm::vec3 CalcLightOuput(const glm::vec3& ray_dir, const glm::vec3& intersection_point, const Light& light) const;

      glm::vec3 GetAmbientlight() const { return m_Amb; }

      class Builder : private BuilderUtility
      {
      public:
         Builder() = default;
         Builder(const Builder&) = delete;
         void operator=(const Builder&) = delete;

         const Builder& ParseTriangle(const std::string& data);
         Triangle GetTriangle() const { return Triangle(m_Vert1, m_Vert2, m_Vert3, m_Amb, m_Dif, m_Spe, m_Shine); }

      private:
         glm::vec3 m_Vert1;
         glm::vec3 m_Vert2;
         glm::vec3 m_Vert3;
         glm::vec3 m_Amb;
         glm::vec3 m_Dif;
         glm::vec3 m_Spe;
         float m_Shine;

         const char* V1 = "v1:";
         const char* V2 = "v2:";
         const char* V3 = "v3:";
      };

private:
   glm::vec3 m_Vert1;
   glm::vec3 m_Vert2;
   glm::vec3 m_Vert3;
   glm::vec3 m_Amb;
   glm::vec3 m_Dif;
   glm::vec3 m_Spe;
   float m_Shine;
};
