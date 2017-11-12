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

static const float LIGHT_BIAS = 1e-3f;

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

      {
         std::string triangle_attr = "";
         while ((triangle_attr = GetAttributes("triangle")) != "")
         {
            m_Triangles.push_back(Triangle::Builder().ParseTriangle(triangle_attr).GetTriangle());
         }
      }

      {
         std::string plane_attr = "";
         while ((plane_attr = GetAttributes("plane")) != "")
         {
            m_Planes.push_back(Plane::Builder().ParsePlane(plane_attr).GetPlane());
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

         if (target.m_ObjType != IntersectingObject::INVALID)
         {
            for (Light light : m_Lights)
            {

               switch (target.m_ObjType)
               {
               case IntersectingObject::SPHERE:
                  pixelColor += target.m_Sphere.GetAmbientlight();
                  if (!IsLightObstructed(&light, &target))
                  {
                     pixelColor += target.m_Sphere.CalcLightOuput(rayDirection, target.m_Point, light);
                  }
                  break;
               case IntersectingObject::TRIANGLE:
                  if (IsLightObstructed(&light, &target))
                  {
                     pixelColor += target.m_Triangle.GetAmbientlight();
                  }
                  else
                  {
                     glm::vec3 line1 = target.m_Triangle.GetVertexTwo() - target.m_Triangle.GetVertexOne();
                     glm::vec3 line2 = target.m_Triangle.GetVertexThree() - target.m_Triangle.GetVertexOne();
                     glm::vec3 normal = -glm::normalize(glm::cross(line1, line2));

                     glm::vec3 v = -rayDirection;
                     glm::vec3 light_direction = glm::normalize(target.m_Point - light.GetPosition());
                     glm::vec3 reflection = glm::reflect(light_direction, normal);
                     float ln = glm::dot(normal, light_direction);
                     float rv = glm::dot(reflection, v);
                     if (ln < 0) { ln = 0; }
                     if (rv < 0) { rv = 0; }
                     rv = std::pow(rv, target.m_Triangle.GetShine());

                     pixelColor += target.m_Triangle.GetAmbientlight();
                     glm::vec3 lightAddition = light.GetColor()*(target.m_Triangle.GetDiffusion()*ln + target.m_Triangle.GetSpecular()*rv);
                     pixelColor += lightAddition;
                  }
                  break;
               case IntersectingObject::PLANE:
                  if (IsLightObstructed(&light, &target))
                  {
                     pixelColor += target.m_Plane.GetAmbientlight();
                  }
                  else
                  {
                     glm::vec3 normal = glm::normalize(target.m_Plane.GetNormal());
                     glm::vec3 v = -rayDirection;
                     glm::vec3 light_direction = glm::normalize(target.m_Point - light.GetPosition());
                     glm::vec3 reflection = glm::reflect(light_direction, normal);
                     float ln = glm::dot(normal, light_direction);
                     float rv = glm::dot(reflection, v);
                     if (ln < 0) { ln = 0; }
                     if (rv < 0) { rv = 0; }

                     rv = std::pow(rv, target.m_Plane.GetShine());
                     pixelColor += target.m_Plane.GetAmbientlight();
                     glm::vec3 lightAddition = light.GetColor()*(target.m_Plane.GetDiffusion()*ln + target.m_Plane.GetSpecular()*rv);
                     pixelColor += lightAddition;

                  }
                  break;
               default:
                  break;
               }
            }

         }

         float color[3] = { pixelColor.r, pixelColor.g, pixelColor.b };
         m_Img.draw_point(i, j, color);
      }
   }

   m_Img.normalize(0, 255);
   m_Img.save("render2.bmp", true);
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
         if (target.m_ObjType == IntersectingObject::INVALID || distance < target.m_Distance)
         {
            target = IntersectingObject(intersectpoint, distance, sphere);
         }
      }
   }

   for (Triangle triangle : m_Triangles)
   {
      float distance;
      glm::vec3 intersectpoint;

      if(triangle.TestIntersection(m_Camera.GetPosition(), ray_dir, &intersectpoint, &distance))
      {
         if (target.m_ObjType == IntersectingObject::INVALID || distance < target.m_Distance)
         {
            target = IntersectingObject(intersectpoint, distance, triangle);
         }
      }
   }

   for (Plane plane : m_Planes)
   {
      float distance;
      glm::vec3 intersectpoint;

      if (plane.TestIntersection(m_Camera.GetPosition(), ray_dir, &intersectpoint, &distance))
      {
         if (target.m_ObjType == IntersectingObject::INVALID || distance < target.m_Distance)
         {
            target = IntersectingObject(intersectpoint, distance, plane);
         }
      }
   }

   return target;
}

bool Scene::IsLightObstructed(Light* light, IntersectingObject* target)
{
   glm::vec3 lightRay = glm::normalize(light->GetPosition() - target->m_Point);
   glm::vec3 lightRayWithBias = (LIGHT_BIAS * lightRay) + target->m_Point;

   float distance;
   glm::vec3 intersectpoint;

   for (Sphere sphere : m_Spheres)
   {
      if (sphere.TestIntersection(lightRayWithBias, lightRay, &intersectpoint, &distance)) return true;
   }

   for (Triangle triangle : m_Triangles)
   {
      if (triangle.TestIntersection(lightRayWithBias, lightRay, &intersectpoint, &distance)) return true;
   }

   for (Plane plane : m_Planes)
   {
      if(plane.TestIntersection(lightRayWithBias, lightRay, &intersectpoint, &distance)) return true;
   }

   return false;
}
