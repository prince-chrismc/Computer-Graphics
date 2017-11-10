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
#include "Model.h"

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
      std::vector<Model> m_Models;
      std::vector<Triangle> m_Triangles;

      cimg_library::CImg<float> m_Img;

      struct IntersectingObject
      {
         enum ObjectType { INVALID, SPHERE, TRIANGLE };

         IntersectingObject(glm::vec3 point, float dis, void* obj, ObjectType type) : m_Point(point), m_Distance(dis), m_Object(obj), m_ObjType(type) {}
         IntersectingObject() : IntersectingObject(glm::vec3(0.0f), 0.0f, nullptr, IntersectingObject::INVALID) {}

         glm::vec3 m_Point;
         float m_Distance;
         void* m_Object;
         ObjectType m_ObjType;
      };

      void GenerateScene();
      IntersectingObject FindNearestIntersectingObject(glm::vec3 ray_dir);
};
