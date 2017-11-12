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
