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

#include <vector>

class SceneFile
{
   public:
      SceneFile(const char* path);

      std::string GetAttributes(const char* name);

   protected:
      class ObjDescriptor
      {
         public:
            ObjDescriptor(const char* name, const std::string& attributes) : m_Name(name), m_Attributes(attributes) {}

            bool DoesNameMatch(const char* name) const { return !std::string(name).compare(m_Name); }
            std::string GetAttributes() const { return m_Attributes; }

         private:
            const char* m_Name;
            std::string m_Attributes;
      };

      std::vector<ObjDescriptor> m_Elements;

   private:
      void ExtractCamera(std::ifstream* file);
      void ExtractSphere(std::ifstream* file);
      void ExtractModel(std::ifstream* file);
      void ExtractLight(std::ifstream* file);
      void ExtractTriangle(std::ifstream* file);
      void ExtractPlane(std::ifstream* file);

      static std::string GetNextLine(std::ifstream* file);
};
