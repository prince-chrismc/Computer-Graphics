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

#include "Loader.h"
#include <sstream>
#include <future>

void Obj::Loader::ExtractDate()
{
   /// This code can NOT be parallelized because the order needs to be maintained
   for (auto line : m_File.m_Lines)
   {
      switch (line.at(0))
      {
      case 'v':
         switch (line.at(1))
         {
         case ' ': ExtractVertice(line.substr((size_t)Offset::VERTICE));
         case 'n': ExtractNormal(line.substr((size_t)Offset::NORMAL));
         case 't': ExtractTexture(line.substr((size_t)Offset::TEXTURE));
         default:
            break;
         }
      case 'f': m_Indicies.emplace_back(line.substr((size_t)Offset::INDEX));
      default:
         break;
      }
   }

   ExtractIndexing();
}

void Obj::Loader::ExtractVertice(const std::string & line)
{
   float buffer[3];
   std::stringstream ss(line);
   for (int i = 0; i < 3; i++)
      ss >> buffer[i];

   m_Model.m_Vertices.emplace_back(buffer[0], buffer[1], buffer[2]);
}

void Obj::Loader::ExtractNormal(const std::string & line)
{
   float buffer[3];
   std::stringstream ss(line);;
   for (int i = 0; i < 3; i++)
      ss >> buffer[i];

   m_Model.m_Normals.emplace_back(buffer[0], buffer[1], buffer[2]);
}

void Obj::Loader::ExtractTexture(const std::string & line)
{
   float buffer[2];
   std::stringstream ss(line);
   for (int i = 0; i < 2; i++)
      ss >> buffer[i];

   m_Model.m_Textures.emplace_back(buffer[0], buffer[1]);
}

void Obj::Loader::ExtractIndexing()
{
   for (auto line : m_Indicies)
   {
      std::stringstream ss(line);
      unsigned int vertex_index[3], uv_index[3], normal_index[3];
      for (int i = 0; i < 3; i++)
      {
         ss >> vertex_index[i];
         ss >> uv_index[i];
         ss >> normal_index[i];
      }

      for (auto vertex : vertex_index) m_VerticeIndicies.push_back(vertex);
      for (auto uv : uv_index) m_TextureIndicies.push_back(uv);
      for (auto normal : normal_index) m_NormalsIndicies.push_back(normal);
   }
}

void Obj::Loader::SortCoords()
{
   auto temp_vertices = std::async(std::launch::async, [this] {
      std::vector<glm::vec3> temp_vertices;
      for (unsigned int i = 0; i < m_VerticeIndicies.size(); i++)
         temp_vertices.push_back(m_Model.m_Vertices.at(m_VerticeIndicies.at(i) - 1));
      return temp_vertices;
   });

   auto temp_uvs = std::async(std::launch::async, [this] {
   std::vector<glm::vec2> temp_uvs;
   for (unsigned int i = 0; i < m_TextureIndicies.size(); i++)
      temp_uvs.push_back(m_Model.m_Textures.at(m_TextureIndicies.at(i) - 1));
   return temp_uvs;
   });

   auto temp_normals = std::async(std::launch::async, [this] {
   std::vector<glm::vec3> temp_normals;
   for (unsigned int i = 0; i < m_NormalsIndicies.size(); i++)
      temp_normals.push_back(m_Model.m_Normals.at(m_NormalsIndicies.at(i) - 1));
   return temp_normals;
   });

   m_Model.m_Vertices = temp_vertices.get();
   m_Model.m_Textures = temp_uvs.get();
   m_Model.m_Normals = temp_normals.get();
}
