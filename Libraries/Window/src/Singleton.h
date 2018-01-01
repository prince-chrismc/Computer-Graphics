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

#include<mutex>

class GlfwWindow : public IWindow
{
public:
   ~GlfwWindow() = default;
   GlfwWindow(const GlfwWindow&) = delete;
   GlfwWindow& operator=(const GlfwWindow&) = delete;

   static std::shared_ptr<GlfwWindow> CreateInstance(const char* title, const int& width = DEFAULT_WIDTH, const int& height = DEFAULT_HEIGHT);
   static std::shared_ptr<GlfwWindow> GetInstance() { return s_Instance; }

   enum : unsigned int { DEFAULT_WIDTH = 800, DEFAULT_HEIGHT = 600 };            // Default window dimensions

private:
   GlfwWindow(const char* title, const int& width, const int& height) : IWindow(title, width, height) {}
   GlfwWindow(const char* title) : GlfwWindow(title, DEFAULT_WIDTH, DEFAULT_HEIGHT) {}

   static std::once_flag s_Flag;
   static std::shared_ptr<GlfwWindow> s_Instance;
};

#else
#error "Can NOT include both singleton and factory implementations"
#endif // WINDOW_PROGRAM
