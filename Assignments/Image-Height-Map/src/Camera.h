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

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp" //glm::lookAt

class Camera final
{
public:
   Camera(const glm::vec3 center, const glm::vec3 up, const glm::vec3 eye);
   Camera() : Camera(glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0), glm::vec3(-1.0, -1.0, -1.0)) {}

   Camera& operator=(const Camera& camera);

   void AdjustTranslateXAxis(const float& adj) { m_Trans.x += adj; RecalculateCameraPos(); }
   void AdjustTranslateYAxis(const float& adj) { m_Trans.y += adj; RecalculateCameraPos(); }
   void AdjustTranslateZAxis(const float& adj) { m_Trans.z += adj; RecalculateCameraPos(); }

   void AdjustRotateXAxis(const float& adj) { m_Rotate.x += adj; ValidateRotateX(); RecalculateCameraPos(); }
   void AdjustRotateYAxis(const float& adj) { m_Rotate.y += adj; ValidateRotateY(); RecalculateCameraPos(); }
   void AdjustRotateZAxis(const float& adj) { m_Rotate.z += adj; ValidateRotateZ(); RecalculateCameraPos(); }

   void AdjustScalar(const float& adj);

   void ResetCameraPos() { m_Trans = glm::vec3(0.0f); m_Rotate = glm::vec3(0.0f); m_Scalar = 0.0f; RecalculateCameraPos(); }

   glm::mat4 GetViewMatrix() { return m_ViewMatrix; }

protected:
   void RecalculateCameraPos();
   void ValidateRotateX();
   void ValidateRotateY();
   void ValidateRotateZ();

   static constexpr float MAX_ROTATION = 89.0f;
   static constexpr float MIN_ROTATION = -89.0f;

private:
   // home components
   glm::vec3 m_center;
   glm::vec3 m_up;
   glm::vec3 m_eye;

   // adjustment components
   glm::vec3 m_Trans;
   glm::vec3 m_Rotate;
   float m_Scalar;

   glm::mat4 m_ViewMatrix;
};
