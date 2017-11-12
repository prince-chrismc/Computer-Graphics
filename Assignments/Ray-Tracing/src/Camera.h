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
#include "glm\vec3.hpp"
#include <string>

class Camera
{
   public:
      Camera(const glm::vec3& pos, const unsigned int& FOV, const unsigned int& focal, const double& angle) :
             m_Pos(pos), m_FOV(FOV), m_Focal(focal), m_AspectRatio(angle) {}
      Camera() : Camera(glm::vec3(), 0, 0, 0.0) {}

      void GetImageDimensions(int* out_width, int* out_height) const;
      glm::vec3 GetPosition() const { return m_Pos; }
      double GetAspectRatio() const { return m_AspectRatio; }
      unsigned int GetFeildOfView() const { return m_FOV; }

      class Builder : private BuilderUtility
      {
         public:
            Builder() = default;
            Builder(const Builder&) = delete;
            void operator=(const Builder&) = delete;

            Builder& ParseCamera(const std::string& data);
            Camera GetCamera() const { return Camera(m_Pos, m_FOV, m_Focal, m_AspectRatio); }

         private:
            glm::vec3 m_Pos;
            unsigned int m_FOV;
            unsigned int m_Focal;
            double m_AspectRatio;
      };

   private:
      glm::vec3 m_Pos;
      unsigned int m_FOV;
      unsigned int m_Focal;
      double m_AspectRatio;
};
