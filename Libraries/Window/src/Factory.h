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

#include "Window.h"

#ifndef WINDOW_PROGRAM
#define WINDOW_PROGRAM // To prevent inclusion of both Singleton and Multiple implementations

#include <vector>
#include <mutex>

class GlfwWindow : public IWindow
{
friend class GlfwWindowFactory;

public:
   GlfwWindow(const char* title, const int& width, const int& height) : IWindow(title, width, height) {}
   GlfwWindow(const char* title) : GlfwWindow(title, DEFAULT_WIDTH, DEFAULT_HEIGHT) {}
};

class GlfwWindowFactory
{
public:
   ~GlfwWindowFactory() { glfwTerminate(); m_Windows.clear(); }
   GlfwWindowFactory(const GlfwWindowFactory&) = delete;
   GlfwWindowFactory& operator=(const GlfwWindowFactory&) = delete;

   static std::shared_ptr<GlfwWindowFactory> GetInstance() { std::call_once(s_Flag, []() { s_Instance.reset(new GlfwWindowFactory()); }); return s_Instance; }

   std::shared_ptr<GlfwWindow> CreateNewWindow(const char* title, const int& width, const int& height) { m_Windows.push_back(std::make_shared<GlfwWindow>(title, width, height)); return m_Windows.back(); }
   std::shared_ptr<GlfwWindow> CreateNewWindow(const char* title) { m_Windows.push_back(std::make_shared<GlfwWindow>(title)); return m_Windows.back(); }
   std::shared_ptr<GlfwWindow> FindWindow(GLFWwindow* window) const { for (std::shared_ptr<GlfwWindow> win : m_Windows) { if (win->m_Window == window) return win; } return nullptr; }

private:
   GlfwWindowFactory() = default;

   static std::once_flag s_Flag;                                           // http://cppisland.com/?p=501
   static std::shared_ptr<GlfwWindowFactory> s_Instance;                   // https://stackoverflow.com/questions/6876751/differences-between-unique-ptr-and-shared-ptr

   std::vector<std::shared_ptr<GlfwWindow>> m_Windows;
};

#endif // WINDOW_PROGRAM
