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

#include "ObjLoader.h"
#include <vector>
#include <cstdio>
#include <string>

bool LoadObjFile(const char* path, std::vector<glm::vec3>* out_vertices, std::vector<glm::vec3>* out_normals,
   std::vector<glm::vec2>* out_uvs)
{
   std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
   std::vector<glm::vec3> temp_vertices;
   std::vector<glm::vec2> temp_uvs;
   std::vector<glm::vec3> temp_normals;

   std::FILE* file = std::fopen(path, "r");
   if (!file)
   {
      printf("Impossible to open the file! Are you in the right path ?\n");
      getchar();
      return false;
   }

   while (1)
   {
      char lineHeader[128];
      // read the first word of the line
      int res = fscanf_s(file, "%s", lineHeader, (unsigned int)_countof(lineHeader));

      if (res == EOF) break; // EOF = End Of File. Quit the loop.

      if (strcmp(lineHeader, "v") == 0)
      {
         float x, y, z;
         int matches = fscanf_s(file, "%f %f %f\n", &x, &y, &z);
         if (matches != 3)
         {
            printf("File can't be read by our simple parser :-( Try exporting with other options\n");
            return false;
         }
         temp_vertices.emplace_back(x, y, z);
      }
      else if (strcmp(lineHeader, "vt") == 0)
      {
         float x, y;
         int matches = fscanf_s(file, "%f %f\n", &x, &y);
         if (matches != 2)
         {
            printf("File can't be read by our simple parser :-( Try exporting with other options\n");
            return false;
         }
         temp_uvs.emplace_back(x, -y); // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
      }
      else if (strcmp(lineHeader, "vn") == 0)
      {
         float x, y, z;
         int matches = fscanf_s(file, "%f %f %f\n", &x, &y, &z);
         if (matches != 3)
         {
            printf("File can't be read by our simple parser :-( Try exporting with other options\n");
            return false;
         }
         temp_normals.emplace_back(x, y, z);
      }
      else if (strcmp(lineHeader, "f") == 0)
      {
         unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
         int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
         if (matches != 9)
         {
            printf("File can't be read by our simple parser :-( Try exporting with other options\n");
            return false;
         }

         for (unsigned int index : vertexIndex) { vertexIndices.push_back(index); }
         for (unsigned int uv : uvIndex) { uvIndices.push_back(uv); }
         for (unsigned int normal : normalIndex) { normalIndices.push_back(normal); }
      }
      else
      {
         // Probably a comment, eat up the rest of the line
         char stupidBuffer[1000];
         fgets(stupidBuffer, 1000, file);
      }
   }

   // For each vertex of each triangle
   for (unsigned int i = 0; i < vertexIndices.size(); i++)
   {
      // Get the attributes in draw order thanks to the index
      out_vertices->push_back(temp_vertices.at(vertexIndices.at(i) - 1));
      out_uvs->push_back(temp_uvs.at(uvIndices.at(i) - 1));
      out_normals->push_back(temp_normals.at(normalIndices.at(i) - 1));
   }

   return true;
}
