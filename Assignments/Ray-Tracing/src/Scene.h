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

#include "SceneFile.h"
#include "Camera.h"
#include "Light.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"

#include "CImg.h"

class Scene : private SceneFile
{
   public:
      Scene(const char* path);

      void Display();

   private:
      Camera m_Camera;
      std::vector<Light> m_Lights;
      std::vector<Sphere> m_Spheres;
      std::vector<Plane> m_Planes;
      std::vector<Triangle> m_Triangles;

      cimg_library::CImg<float> m_Img;

      struct IntersectingObject
      {
         enum ObjectType { INVALID, SPHERE, TRIANGLE, PLANE };

         IntersectingObject(glm::vec3 point, float dis, Sphere& sphere)     : m_Point(point), m_Distance(dis), m_Sphere(sphere),     m_ObjType(IntersectingObject::SPHERE) {}
         IntersectingObject(glm::vec3 point, float dis, Triangle& triangle) : m_Point(point), m_Distance(dis), m_Triangle(triangle), m_ObjType(IntersectingObject::TRIANGLE) {}
         IntersectingObject(glm::vec3 point, float dis, Plane& plane)       : m_Point(point), m_Distance(dis), m_Plane(plane),       m_ObjType(IntersectingObject::PLANE) {}
         IntersectingObject() : m_Point(0.0f), m_Distance(0.0f), m_Sphere(), m_Triangle(), m_Plane(), m_ObjType(IntersectingObject::INVALID) {}

         glm::vec3 m_Point;
         float m_Distance;
         Sphere m_Sphere;
         Triangle m_Triangle;
         Plane m_Plane;
         ObjectType m_ObjType;
      };

      void GenerateScene();
      IntersectingObject FindNearestIntersectingObject(glm::vec3 ray_dir);
      bool IsLightObstructed(Light* light, IntersectingObject* target);
};
