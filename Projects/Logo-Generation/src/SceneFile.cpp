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

#include "SceneFile.h"
#include <fstream>
#include <sstream>

// Element types
static const char* CAMERA = "camera";
static const char* SPHERE = "sphere";
static const char* MODEL  = "model";
static const char* LIGHT = "light";
static const char* PLANE = "plane";
static const char* TRIANGLE  = "triangle";

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

      if (!line.compare(CAMERA))        ExtractCamera(&file);
      else if (!line.compare(SPHERE))   ExtractSphere(&file);
      else if (!line.compare(MODEL))    ExtractModel(&file);
      else if (!line.compare(LIGHT))    ExtractLight(&file);
      else if (!line.compare(TRIANGLE)) ExtractTriangle(&file);
      else if (!line.compare(PLANE))    ExtractPlane(&file);
   }

   if(m_Elements.size() != num_elem) m_Elements.clear();
}

std::string SceneFile::GetAttributes(const char* name)
{
   std::string retval = "";
   for (auto itor = m_Elements.begin(); itor != m_Elements.end(); itor++)
   {
      if (itor->DoesNameMatch(name))
      {
         retval = itor->GetAttributes();;
         itor = m_Elements.erase(itor);
         break;
      }
   }
   return retval;
}

void SceneFile::ExtractCamera(std::ifstream* file)
{
   std::string pos = GetNextLine(file);
   std::string fov = GetNextLine(file);
   std::string f = GetNextLine(file);
   std::string a = GetNextLine(file);

   m_Elements.emplace_back(CAMERA, "{ " + pos + "," + fov + "," + f + "," + a + " }");
}

void SceneFile::ExtractSphere(std::ifstream * file)
{
   std::string path = GetNextLine(file);
   std::string rad = GetNextLine(file);
   std::string amb = GetNextLine(file);
   std::string dif = GetNextLine(file);
   std::string spe = GetNextLine(file);
   std::string shi = GetNextLine(file);

   m_Elements.emplace_back(SPHERE, "{ " + path + "," + rad + "," + amb + "," + dif + "," + spe + shi + " }");
}

void SceneFile::ExtractModel(std::ifstream * file)
{
   std::string path = GetNextLine(file);
   std::string amb = GetNextLine(file);
   std::string dif = GetNextLine(file);
   std::string spe = GetNextLine(file);
   std::string shi = GetNextLine(file);

   m_Elements.emplace_back(MODEL, "{ " + path + "," + amb + "," + dif + "," + spe + shi + " }");
}

void SceneFile::ExtractLight(std::ifstream * file)
{
   std::string pos = GetNextLine(file);
   std::string col = GetNextLine(file);

   m_Elements.emplace_back(LIGHT, "{ " + pos + "," + col + " }");
}

void SceneFile::ExtractTriangle(std::ifstream * file)
{
   std::string v1 = GetNextLine(file);
   std::string v2 = GetNextLine(file);
   std::string v3 = GetNextLine(file);
   std::string amb = GetNextLine(file);
   std::string dif = GetNextLine(file);
   std::string spe = GetNextLine(file);
   std::string shi = GetNextLine(file);

   m_Elements.emplace_back(TRIANGLE, "{ " + v1 + "," + v2 + "," + v3 + "," + amb + "," + dif + "," + spe + shi + " }");
}

void SceneFile::ExtractPlane(std::ifstream * file)
{
   std::string nor = GetNextLine(file);
   std::string pos = GetNextLine(file);
   std::string amb = GetNextLine(file);
   std::string dif = GetNextLine(file);
   std::string spe = GetNextLine(file);
   std::string shi = GetNextLine(file);

   m_Elements.emplace_back(PLANE, "{ " + nor + "," + pos + "," + amb + "," + dif + "," + spe + shi + " }");
}

std::string SceneFile::GetNextLine(std::ifstream* file)
{
   char buffer[512];
   file->getline(buffer, 512, '\n');

   return std::string(buffer);
}
