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

#include "Camera.h"
#include "glm\trigonometric.hpp"

Camera::Builder& Camera::Builder::ParseCamera(const json& data)
{
   if (!data.is_object()) throw std::exception("Invalid format for camera element - not an object");
   if (data.size() != 4)  throw std::exception("Invalid format for camera element - size not 4");

   m_Pos = ParseVec3(data[POS]);
   m_FOV = ParseUint(data[FOV]);
   m_Focal = ParseUint(data[F]);
   m_AspectRatio = ParseDouble(data[A]);

   return *this;
}

void Camera::GetImageDimensions(int* out_width, int* out_height) const
{
   *out_height = 2 * static_cast<int>(m_Focal * std::tan(glm::radians(m_FOV / 2.0)));
   *out_width = static_cast<int>(*out_height * m_AspectRatio);
}
