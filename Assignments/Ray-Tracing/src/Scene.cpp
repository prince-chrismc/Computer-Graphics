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

#include "Scene.h"

#include "glm\geometric.hpp"

#include <string>
#include <future>
#include <corecrt_math_defines.h>

using cimg_library::CImg;
using cimg_library::CImgDisplay;

static const float LIGHT_BIAS = 1e-3;

Scene::Scene(const char* path) : SceneFile(path)
{
   if(m_Elements.size() > 0)
   {
      // Do Work
      m_Camera = Camera::Builder().ParseCamera(GetAttributes("camera")).GetCamera();

      {
         std::string light_attr = "";
         while ((light_attr = GetAttributes("light")) != "")
         {
            m_Lights.push_back(Light::Builder().ParseLight(light_attr).GetLight());
         }
      }

      {
         std::string sphere_attr = "";
         while ((sphere_attr = GetAttributes("sphere")) != "")
         {
            m_Spheres.push_back(Sphere::Builder().ParseSphere(sphere_attr).GetSphere());
         }
      }

      GenerateScene();
   }
}

void Scene::Display()
{
   CImgDisplay display(m_Img, "Image");

   while(!display.is_closed())
   {
      if(display.is_keyESC()) display.close();
   }
}

void Scene::GenerateScene()
{
   int width = 0, height = 0;
   m_Camera.GetImageDimensions(&width, &height);
   m_Img = CImg<float>(width, height, 1, 3, 0);

   for (int i = 0; i < width; i += 1)
   {
      for (int j = 0; j < height; j += 1)
      {
         // Calc direction of ray
         double PCx = (2.0 * ((i + 0.5) / width) - 1.0) * tan(m_Camera.GetFeildOfView() / 2.0 * M_PI / 180.0) * m_Camera.GetAspectRatio();
         double PCy = (1.0 - 2.0 * ((j + 0.5) / height)) * tan(m_Camera.GetFeildOfView() / 2.0 * M_PI / 180.0);
         glm::vec3 rayDirection = glm::normalize(glm::vec3(PCx, PCy, -1) - m_Camera.GetPosition());

         IntersectingObject target = FindNearestIntersectingObject(rayDirection);
         glm::vec3 pixelColor;

         if (target.m_Object != nullptr)
         {
            /*for (Light light : m_Lights)
            {
               glm::vec3 lightRay = glm::normalize(light.GetPosition() - target.m_Point);
               glm::vec3 lightRayWithBias = target.m_Point + LIGHT_BIAS * lightRay;

               switch (target.m_ObjType)
               {
               case IntersectingObject::SPHERE:
                  float distance;
                  glm::vec3 intersectpoint;
                  ((Sphere*)target.m_Object)->TestIntersection(lightRayWithBias, lightRay, &intersectpoint, &distance);

                  break;
               case IntersectingObject::TRIANGLE:
                  float distance;
                  glm::vec3 intersectpoint;
                  ((Triangle*)target.m_Object)->TestIntersection(lightRayWithBias, lightRay, &intersectpoint, &distance);
                  break;

               default:
                  break;
               }
            }*/

            pixelColor = glm::vec3(0.3,0.1,0.8);
         }

         float color[3] = { pixelColor.r, pixelColor.g, pixelColor.b };
         m_Img.draw_point(i, j, color);
      }
   }

   m_Img.normalize(0, 255);
   m_Img.save("render.bmp", true);
}

Scene::IntersectingObject Scene::FindNearestIntersectingObject(glm::vec3 ray_dir)
{
   IntersectingObject target;

   for (Sphere sphere : m_Spheres)
   {
      float distance;
      glm::vec3 intersectpoint;

      if (sphere.TestIntersection(m_Camera.GetPosition(), ray_dir, &intersectpoint, &distance))
      {
         if (target.m_Object == nullptr || distance < target.m_Distance)
         {
            target = IntersectingObject(intersectpoint, distance, &sphere, IntersectingObject::SPHERE);
         }
      }
   }

   for (Triangle triangle : m_Triangles)
   {
      float distance;
      glm::vec3 intersectpoint;

      if(triangle.TestIntersection(m_Camera.GetPosition(), ray_dir, &intersectpoint, &distance))
      {
         if (target.m_Object == nullptr || distance < target.m_Distance)
         {
            target = IntersectingObject(intersectpoint, distance, &triangle, IntersectingObject::SPHERE);
         }
      }
   }

   return target;
}
