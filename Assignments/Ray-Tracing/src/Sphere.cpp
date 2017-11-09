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

#include "Sphere.h"

#include "glm\geometric.hpp"   //normalize

bool Sphere::TestIntersection(const glm::vec3& cam_pos, const glm::vec3& ray_dir, glm::vec3* out_intersection, float* out_distance) const
{
   // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection

   float intersection_zero, intersection_one;
   glm::vec3 length = m_Pos - cam_pos;
   float tca = glm::dot(length, ray_dir);

   float rad_dir_squared = glm::dot(length, length) - tca * tca;
   if (rad_dir_squared > (m_Radius * m_Radius)) return false; // doesn't pass through

   float thc = sqrt((m_Radius * m_Radius) - rad_dir_squared);
   intersection_zero = tca - thc;
   intersection_one = tca + thc;

   if (intersection_zero > intersection_one) std::swap(intersection_zero, intersection_one);

   if (intersection_zero < 0)
   {
      intersection_zero = intersection_one;
      if (intersection_zero < 0) return false; // both intersection are negative
   }

   *out_distance = intersection_zero;
   *out_intersection = cam_pos + ray_dir * intersection_zero;

   return true;
}
