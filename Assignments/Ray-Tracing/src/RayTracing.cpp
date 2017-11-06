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
#include <iostream>
#include <sstream>

#include "glm/gtc/matrix_transform.hpp" //glm::lookAt, scale, etc...
#include "CImg.h"

using cimg_library::CImg;
using cimg_library::CImgDisplay;

// Function Declaration
const unsigned long CalcHexColorFromPixelVal(const float& pixel_value);

int main()
{
   std::cout << "Welcome to Ray Tracing Effect!" << std::endl;

   // Constant vectors
   const glm::vec3 center(0.0f, 0.0f, 0.0f);
   const glm::vec3 up(0.0f, 1.0f, 0.0f);
   const glm::vec3 eye(0.0f, 35.0f, 35.0f);

   // ---------------------------------------------------------------------------------------------
   std::cout << "Generating Scene....";
   CImg<float> image;

   /*
   * TO DO: Generate Scene as per assignemnt description
   */

   CImgDisplay display(image, "Image");           // create window displaying image

   return 0;
}

// ------------------------------------------------------------------------------------------------ //
//                                          UTILITY CODE                                          - //
// ------------------------------------------------------------------------------------------------ //

const unsigned long CalcHexColorFromPixelVal(const float & pixel_value)
{
   return static_cast<unsigned long>(std::floor(std::pow(pixel_value, 2.0))); // blue to green
}

float GetUserInputFraction(const float& precision)
{
   float selection = 0.0;
   do
   {
      std::cout << "Selcetion: ";
      std::string input;
      std::getline(std::cin, input);
      std::stringstream ss(input);
      ss >> selection;

      if (selection < 0.0 || selection > 1.0 )
         std::cout << "Invalid option. Please Try again..." << std::endl;

   } while (selection < 0.0 || selection > 1.0);

   return floor(selection / precision + 0.5f) * precision; // https://stackoverflow.com/a/798070/8480874
}
