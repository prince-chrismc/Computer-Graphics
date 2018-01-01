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
#include <map>

namespace Shader
{
   // Allows for multiple shader programs to exist
   class Program final : public IProgram
   {
      // Everything is defined in the base class
   };

   class Map final
   {
   public:
      ~Map() = default;
      Map(const Map&) = delete;
      Map& operator=(const Map&) = delete;

      static std::shared_ptr<Map> GetInstance() { std::call_once(s_Flag, []() { s_Instance.reset(new Map()); }); return s_Instance; }

      std::shared_ptr<Program> GetProgram(const unsigned long& id) const { try { return m_IdAndPrograms.at(id); } catch(std::out_of_range) { return nullptr; } }
      std::shared_ptr<Program> MakeProgram(const unsigned long& id) { std::shared_ptr<Program> prg = std::make_shared<Program>(); if(m_IdAndPrograms.emplace(id, prg).second) return prg; else return nullptr; }

   private:
      Map() = default;

      std::map<unsigned long, std::shared_ptr<Program>> m_IdAndPrograms;

      static std::once_flag s_Flag;
      static std::shared_ptr<Map> s_Instance;
   };
}

#else
#error "Can NOT include both singleton and factory implementations"
#endif // SHADER_PROGRAM
