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

#include "glm\vec2.hpp"

class Cursor final
{
public:
   Cursor() {}

   static constexpr float Sensitivity = 0.05f;

   void SetLastX(const float& last) { m_Last.x = last; }
   void SetLastY(const float& last) { m_Last.y = last; }
   const float& GetLastX() { return m_Last.x; }
   const float& GetLastY() { return m_Last.y; }

   void ToggleLeftButton() { m_LeftActive ? m_LeftActive = false : m_LeftActive = true; }
   void ToggleMiddleButton() { m_MiddleActive ? m_MiddleActive = false : m_MiddleActive = true; }
   void ToggleRightButton() { m_RightActive ? m_RightActive = false : m_RightActive = true; }
   bool IsLeftActive() { return m_LeftActive; }
   bool IsMiddleActive() { return m_MiddleActive; }
   bool IsRightActive() { return m_RightActive; }

private:
   glm::vec2 m_Last;
   bool m_LeftActive;
   bool m_MiddleActive;
   bool m_RightActive;

};
