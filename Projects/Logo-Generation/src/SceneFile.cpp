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
#include <iostream>
#include "BuilderUtility.h"
#include "BuilderUtility.h"


SceneFile::SceneFile(const char* path)
{
   json json_file;
   {
      std::ifstream raw_file(path);
      raw_file >> json_file;
   }

   if (!json_file.is_array())
   {
      throw std::exception("Invalid scene file format");
   }

   const auto num_elem = json_file.size();

   for (auto& itor : json_file)
   {
      if (!itor.is_object()) throw std::exception("Invalid scene file format");

      for (json::iterator object = itor.begin(); object != itor.end(); ++object)
      {
#ifdef _DEBUG
         const auto key = object.key();
         std::cout << key << ":" << object.value() << std::endl;
#endif
         m_Elements.emplace_back(object.key(), object.value());
      }
   }

   if (m_Elements.size() != num_elem)
   {
      m_Elements.clear();
      throw std::exception("Unknown");
   }
}

json SceneFile::GetAttributes(const char* name)
{
   json retval;

   for (auto itor = m_Elements.begin(); itor != m_Elements.end(); ++itor)
   {
      if (itor->DoesNameMatch(name))
      {
         retval = itor->GetAttributes();
         itor = m_Elements.erase(itor);
         break;
      }
   }

   return retval;
}
