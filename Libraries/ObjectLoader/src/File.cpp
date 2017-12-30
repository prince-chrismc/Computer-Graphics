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

#include "File.h"
#include <fstream>
#include <iostream>
#include <future>
#include <sstream>

ObjFormat::File::File(const char* file_path)
{
   std::ifstream obj_file(file_path, std::ios::in);

   if (!obj_file.is_open())
   {
      std::cout << "Unable to open file: " << file_path << std::endl;
      return;
   }

   std::string line_buffer = "";
   while (std::getline(obj_file, line_buffer))
   {
      m_Lines.push_back(line_buffer);
   }

   obj_file.close();
}

bool ObjFormat::File::ExtractDate()
{
   //for (int x = 0; x + BATCH_SIZE < m_Lines.size(); x += BATCH_SIZE)
   //{
   //   std::async(std::launch::async, [this, x] {
   //      for (auto itor = m_Line.begin() + x; itor != m_Lines.begin() + 2 * x; itor++)
   //      {

   //      }
   //   });
   //}

   for (auto line : m_Lines)
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

   return true;
}

void ObjFormat::File::ExtractVertice(const std::string & line)
{
   float buffer[3];
   std::stringstream ss(line);
   for(int i = 0; i < 3; i++)
      ss >> buffer[i];

   m_Vertices.emplace_back(buffer[0], buffer[1], buffer[2]);
}

void ObjFormat::File::ExtractNormal(const std::string & line)
{
   float buffer[3];
   std::stringstream ss(line);;
      for (int i = 0; i < 3; i++)
         ss >> buffer[i];

   m_Normals.emplace_back(buffer[0], buffer[1], buffer[2]);
}

void ObjFormat::File::ExtractTexture(const std::string & line)
{
   float buffer[2];
   std::stringstream ss(line);
      for (int i = 0; i < 2; i++)
         ss >> buffer[i];

   m_Textures.emplace_back(buffer[0], buffer[1]);
}

void ObjFormat::File::ExtractIndexing()
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
