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

#include "Triangle.h"
#include "Light.h"
#include "glm\geometric.hpp"
#include <algorithm>

bool Triangle::TestIntersection(const glm::vec3& cam_pos, const glm::vec3& ray_dir, glm::vec3* out_intersection, float* out_distance) const
{
   // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

   const double EPSILON = 0.0000001;
   glm::vec3 edge1, edge2, h, s, q;
   float a, f, u, v;

   edge1 = m_Vert2 - m_Vert1;
   edge2 = m_Vert3 - m_Vert1;

   h = glm::cross(ray_dir, edge2);
   a = glm::dot(edge1, h);
   if (a > -EPSILON && a < EPSILON)
      return false;

   f = 1 / a;
   s = cam_pos - m_Vert1;
   u = f * (glm::dot(s, h));
   if (u < 0.0 || u > 1.0)
      return false;

   q = glm::cross(s, edge1);
   v = f * glm::dot(ray_dir, q);
   if (v < 0.0 || u + v > 1.0)
      return false;

   // At this stage we can compute t to find out where the intersection point is on the line.
   float t = f * glm::dot(edge2, q);
   if (t > EPSILON) // ray intersection
   {
      *out_intersection = cam_pos + ray_dir * t;
      *out_distance = t;
      return true;
   }

   // This means that there is a line intersection but not a ray intersection.
   return false;
}

glm::vec3 Triangle::CalcLightOuput(const glm::vec3 & ray_dir, const glm::vec3 & intersection_point, const Light & light) const
{
   glm::vec3 line1 = m_Vert2 - m_Vert1;
   glm::vec3 line2 = m_Vert3 - m_Vert1;
   glm::vec3 normal = -glm::normalize(glm::cross(line1, line2));
   glm::vec3 v = -ray_dir;

   glm::vec3 light_direction = glm::normalize(intersection_point - light.GetPosition());
   glm::vec3 reflection = glm::reflect(light_direction, normal);

   float ln = glm::dot(normal, light_direction);
   float rv = glm::dot(reflection, v);

   ln = std::max(ln, 0.0f);
   rv = std::max(rv, 0.0f);

   rv = std::pow(rv, m_Shine);

   return light.GetColor() * (m_Dif*ln + (m_Spe*rv));
}

const Triangle::Builder& Triangle::Builder::ParseTriangle(const std::string& data)
{
   std::string cut = data.substr(2, data.length() - 4);

   for (std::string attribute : ParseParams(cut))
   {
      if (attribute.find("v1:") == 0)
      {
         m_Vert1 = ParseVec3(attribute.substr(4));
      }
      else if (attribute.find("v2:") == 0)
      {
         m_Vert2 = ParseVec3(attribute.substr(4));
      }
      else if (attribute.find("v3:") == 0)
      {
         m_Vert3 = ParseVec3(attribute.substr(4));
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
         m_Shine = ParseFloat(attribute.substr(5));
      }
   }

   return *this;
}
