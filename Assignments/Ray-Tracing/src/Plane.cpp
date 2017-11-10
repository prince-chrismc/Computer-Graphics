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

#include "Plane.h"
#include "glm\geometric.hpp"

bool Plane::TestIntersection(const glm::vec3& cam_pos, const glm::vec3& ray_dir, glm::vec3* out_intersection, float* out_distance) const
{
   auto normal = glm::normalize(m_Normal);
   float denom = glm::dot(normal, ray_dir);

   if (abs(denom) > 1e-6)
   {
      glm::vec3 p0l0 = m_Pos - ray_dir;

      *out_distance = glm::dot(p0l0, normal) / denom;
      *out_intersection = cam_pos + ray_dir * (*out_distance);

      return (*out_distance >= 0);
   }

   return false;
}

Plane::Builder& Plane::Builder::ParsePlane(const std::string& data)
{
   std::string cut = data.substr(2, data.length() - 4);

   for (std::string attribute : ParseParams(cut))
   {
      if (attribute.find("pos:") == 0)
      {
         m_Pos = ParseVec3(attribute.substr(5));
      }
      else if (attribute.find("nor:") == 0)
      {
         m_Normal = ParseVec3(attribute.substr(5));
      }
      else if (attribute.find("amb:") == 0)
      {
         m_Amb = ParseVec3(attribute.substr(5));
      }
      else if (attribute.find("dif:") == 0)
      {
         m_Dif = ParseVec3(attribute.substr(5));
      }
      else if (attribute.find("spe:") == 0)
      {
         m_Spe = ParseVec3(attribute.substr(5));
      }
      else if (attribute.find("shi:") == 0)
      {
         m_Shine = ParseDouble(attribute.substr(5));
      }
   }

   return *this;
}
