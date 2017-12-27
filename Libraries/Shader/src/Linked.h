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

#include "Interfaces.h"

#ifndef SHADER_PROGRAM
#define SHADER_PROGRAM // To prevent inclusion of both Singleton and Multiple implementations

#include <mutex>

namespace Shader
{
   // A singleton shader program access able everywhere
   class Linked final : protected IProgram
   {
      public:
         ~Linked() = default;
         Linked(const Linked&) = delete;
         Linked& operator=(const Linked&) = delete;

         static std::shared_ptr<Linked> GetInstance() { std::call_once(s_Flag, []() { s_Instance.reset(new Linked()); }); return s_Instance; }

         void Init(IShader* vertex, IShader* frag) { Link(vertex, frag); Activate(); }

         bool operator()() const { return IProgram::operator()(); }

      private:
         Linked() = default;

         static std::once_flag s_Flag;
         static std::shared_ptr<Linked> s_Instance;
   };
}

#endif // SHADER_PROGRAM
