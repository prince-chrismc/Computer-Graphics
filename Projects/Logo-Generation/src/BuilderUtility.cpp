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

#include "BuilderUtility.h"
#include <sstream>

std::vector<std::string> BuilderUtility::ParseParams(std::string cut)
{
   std::vector<std::string> params;
   while (cut.find(',') != std::string::npos)
   {
      size_t index = cut.find(',');
      params.push_back(cut.substr(0, index));
      cut = cut.substr(index + 1);
   }
   params.push_back(cut);

   return params;
}

glm::vec3 BuilderUtility::ParseVec3(std::string attribute)
{
   std::vector<float> vals;
   while (attribute.find(' ') != std::string::npos)
   {
      size_t index = attribute.find(' ');
      std::stringstream ss(attribute.substr(0, index));
      attribute = attribute.substr(index + 1);
      float temp;
      ss >> std::dec >> temp;
      vals.push_back(temp);
   }
   std::stringstream ss(attribute);
   float temp;
   ss >> std::dec >> temp;
   vals.push_back(temp);

   return glm::vec3(vals.at(0), vals.at(1), vals.at(2));
}

unsigned int BuilderUtility::ParseUint(std::string attribute)
{
   unsigned int temp = 0;
   std::stringstream ss(attribute);
   ss >> std::dec >> temp;
   return temp;
}

double BuilderUtility::ParseDouble(std::string attribute)
{
   double temp = 0;
   std::stringstream ss(attribute);
   ss >> std::dec >> temp;
   return temp;
}

float BuilderUtility::ParseFloat(std::string attribute)
{
   float temp = 0;
   std::stringstream ss(attribute);
   ss >> std::dec >> temp;
   return temp;
}
