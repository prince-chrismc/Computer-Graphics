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

#include "GLFW/glfw3.h"                   // include GLFW helper library
#include "glm/detail/type_mat4x4.hpp"     // include glm::mat4 library
#include <mutex>

class IWindow
{
public:
   IWindow(const char* title, const int& width, const int& height);
   IWindow(const char* title) : IWindow(title, DEFAULT_WIDTH, DEFAULT_HEIGHT) {}

   constexpr bool IsValid() const { return m_Window != nullptr; }          // Make sure windows exists
   void NextBuffer() const { glfwSwapBuffers(m_Window); }                  // Swap the screen buffers
   bool ShouldClose() const { return glfwWindowShouldClose(m_Window); }    // window should close
   void CloseWindow() const { glfwSetWindowShouldClose(m_Window, GLFW_TRUE); }

   const glm::mat4& GetProjectionMatrix() const { return m_Projection; }

   // Allow the required callback functions
   GLFWkeyfun         SetKeyCallback(GLFWkeyfun cbfun) { return glfwSetKeyCallback(m_Window, cbfun); }
   GLFWmousebuttonfun SetMouseButtonCallback(GLFWmousebuttonfun cbfun) { return glfwSetMouseButtonCallback(m_Window, cbfun); }
   GLFWcursorposfun   SetCursorPosCallback(GLFWcursorposfun cbfun) { return glfwSetCursorPosCallback(m_Window, cbfun); }
   GLFWwindowsizefun  SetWindowSizeCallback(GLFWwindowsizefun cbfun) { return glfwSetWindowSizeCallback(m_Window, cbfun); }

   static void TriggerCallbacks() { glfwPollEvents(); }                    // For all windows, trigger callback for any pending event

   enum : GLuint { DEFAULT_WIDTH = 1000, DEFAULT_HEIGHT = 700 };            // Default window dimensions

protected:
   GLFWwindow* m_Window;
   glm::mat4   m_Projection;

   static std::once_flag s_InitFlag;

   void UpdateFromResize(const int& width, const int& height);
};

