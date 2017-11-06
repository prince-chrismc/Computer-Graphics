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

#include <fstream>
#include <sstream>
#include "SceneFile.h"

// Element types
static const char* CAMERA = "camera";
static const char* SPHERE = "sphere";
static const char* MODEL  = "model";
static const char* LIGHT  = "light";

SceneFile::SceneFile(const char* path)
{
   std::ifstream file(path);

   unsigned int num_elem = 0;
   {
      char buffer[512];
      file.getline(buffer, 512, '\n');

      std::stringstream line(buffer);
      line >> std::dec >> num_elem;
   }

   while (!file.eof())
   {
      std::string line = GetNextLine(&file);

      if (line.compare(CAMERA))
      {
         std::string pos = GetNextLine(&file);
         std::string fov = GetNextLine(&file);
         std::string f = GetNextLine(&file);
         std::string a = GetNextLine(&file);

         m_Elements.emplace_back(CAMERA, "{ " + pos + "," + fov + "," + f + "," + a + " }");
      }
      else if (line.compare(SPHERE))
      {
         std::string path = GetNextLine(&file);
         std::string rad = GetNextLine(&file);
         std::string amb = GetNextLine(&file);
         std::string dif = GetNextLine(&file);
         std::string spe = GetNextLine(&file);
         std::string shi = GetNextLine(&file);

         m_Elements.emplace_back(SPHERE, "{ " + path + "," + rad + "," + amb + "," + dif + "," + spe + shi +" }");
      }
      else if (line.compare(MODEL))
      {
         std::string path = GetNextLine(&file);
         std::string amb = GetNextLine(&file);
         std::string dif = GetNextLine(&file);
         std::string spe = GetNextLine(&file);
         std::string shi = GetNextLine(&file);

         m_Elements.emplace_back(MODEL, "{ " + path + "," + amb + "," + dif + "," + spe + shi + " }");
      }
      else if (line.compare(LIGHT))
      {
         std::string pos = GetNextLine(&file);
         std::string col = GetNextLine(&file);

         m_Elements.emplace_back(CAMERA, "{ " + pos + "," + col + " }");
      }
   }

}

SceneFile::~SceneFile()
{
}

std::string SceneFile::GetNextLine(std::ifstream* file)
{
   char buffer[512];
   file->getline(buffer, 512, '\n');

   return std::string(buffer);
}
