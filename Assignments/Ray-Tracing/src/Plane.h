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

class Plane : public SceneElement
{
   public:
      Plane() = default;
      Plane(const glm::vec3& pos, const glm::vec3& nor, const glm::vec3& amb, const glm::vec3& dif, const glm::vec3& spe, const float& shine) :
         m_Pos(pos), m_Normal(nor),  m_Amb(amb), m_Dif(dif), m_Spe(spe), m_Shine(shine) {}

      bool TestIntersection(const glm::vec3& cam_pos, const glm::vec3& ray_dir, glm::vec3* out_intersection, float* out_distance) const;
      glm::vec3 CalcLightOuput(const glm::vec3& ray_dir, const glm::vec3& intersection_point, const Light& light) const;

      glm::vec3 GetAmbientlight() const { return m_Amb; }

      class Builder : private BuilderUtility
      {
      public:
         Builder() = default;
         Builder(const Builder&) = delete;
         void operator=(const Builder&) = delete;

         const Builder& ParsePlane(const std::string& data);
         Plane GetPlane() const { return Plane(m_Pos, m_Normal, m_Amb, m_Dif, m_Spe, m_Shine); }

      private:
         glm::vec3 m_Pos;
         glm::vec3 m_Normal;
         glm::vec3 m_Amb;
         glm::vec3 m_Dif;
         glm::vec3 m_Spe;
         float m_Shine;

         const char* NOR = "nor:";
      };

private:
   glm::vec3 m_Pos;
   glm::vec3 m_Normal;
   glm::vec3 m_Amb;
   glm::vec3 m_Dif;
   glm::vec3 m_Spe;
   float m_Shine;
};
