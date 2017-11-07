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

#include <vector>
#include <sstream>
#include "Camera.h"

Camera::Builder& Camera::Builder::ParseCamera(const std::string& data)
{
   std::string cut = data.substr(2, data.length() - 4);

   std::vector<std::string> params;
   while (cut.find(',') != std::string::npos)
   {
      size_t index = cut.find(',');
      params.push_back(cut.substr(0, index));
      cut = cut.substr(index + 1);
   }
   params.push_back(cut);

   for (std::string attribute : params)
   {
      if (attribute.find("pos:") == 0)
      {
         m_Pos = glm::vec3(attribute.at(5) - '0', attribute.at(7) - '0', attribute.back() - '0');
      }
      else if (attribute.find("fov:") == 0)
      {
         std::stringstream fov_val(attribute.substr(5));

         fov_val >> std::dec >> m_FOV;
      }
      else if (attribute.find("f:") == 0)
      {
         std::stringstream f_val(attribute.substr(3));

         f_val >> std::dec >> m_Focal;
      }
      else if (attribute.find("a:") == 0)
      {
         std::stringstream a_val(attribute.substr(3));

         a_val >> std::dec >> m_Angle;
      }
   }

   return *this;
}