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

#include "glm/gtc/matrix_transform.hpp" //glm::lookAt
#include "Camera.h"

Camera::Camera(const glm::vec3 center, const glm::vec3 up, const glm::vec3 eye) : m_center(center), m_up(up), m_eye(eye)
{
   m_ViewMatrix = glm::lookAt(eye, center, up);
}

Camera& Camera::operator=(const Camera& camera)
{
   m_center = camera.m_center;
   m_up = camera.m_up;
   m_eye = camera.m_eye;
   m_Trans = camera.m_Trans;
   m_Rotate = camera.m_Rotate;
   m_Scalar = camera.m_Scalar;

   RecalculateCameraPos();

   return *this;
}

void Camera::AdjustScalar(const float & adj)
{
   m_Scalar += adj;

   if (m_Scalar < 0.5f)
      m_Scalar = 0.5f;

   if (m_Scalar > 5.0f)
      m_Scalar = 5.0f;

   RecalculateCameraPos();
}

void Camera::RecalculateCameraPos()
{
   m_ViewMatrix = glm::lookAt(m_eye, m_center, m_up);
   m_ViewMatrix = glm::translate(m_ViewMatrix, m_Trans);
   m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
   m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
   m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
   m_ViewMatrix = glm::scale(m_ViewMatrix, glm::vec3(m_Scalar));
}

void Camera::ValidateRotateX()
{
   if (m_Rotate.x < MIN_ROTATION) m_Rotate.x = MIN_ROTATION;
   if (m_Rotate.x > MAX_ROTATION) m_Rotate.x = MAX_ROTATION;
}

void Camera::ValidateRotateY()
{
   if (m_Rotate.y < MIN_ROTATION) m_Rotate.y = MIN_ROTATION;
   if (m_Rotate.y > MAX_ROTATION) m_Rotate.y = MAX_ROTATION;
}

void Camera::ValidateRotateZ()
{
   if (m_Rotate.z < MIN_ROTATION) m_Rotate.z = MIN_ROTATION;
   if (m_Rotate.z > MAX_ROTATION) m_Rotate.z = MAX_ROTATION;
}
