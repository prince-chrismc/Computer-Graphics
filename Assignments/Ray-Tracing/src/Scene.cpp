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

#include "Scene.h"

using cimg_library::CImg;
using cimg_library::CImgDisplay;

Scene::Scene(const char* path) : SceneFile(path)
{
   if(m_Elements.size() > 0)
   {
      // Do Work
      m_Camera = Camera::Builder().ParseCamera(GetAttributes("camera")).GetCamera();
      m_Lights.push_back(Light::Builder().ParseLight(GetAttributes("light")).GetLight());

      GenerateScene();
   }
}

void Scene::Display()
{
   CImgDisplay display(m_Img, "Image");

   while(!display.is_closed())
   {
      if(display.is_keyESC()) display.close();
   }
}

void Scene::GenerateScene()
{
   int height = 0, width = 0;
   m_Camera.GetImageDimensions(&width, &height);
   m_Img = CImg<float>(height, width, 1, 3, 0);
}
