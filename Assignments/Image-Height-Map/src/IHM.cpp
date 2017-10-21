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

#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <algorithm>

#include "glm/gtc/matrix_transform.hpp" //glm::lookAt, scale, etc...
#include "GL/glew.h"                    // include GL Extension Wrangler
#include "CImg.h"

#include "GlfwWindow.h"
#include "Shader.h"
#include "Camera.h"
#include "Cursor.h"
#include "RenderMode.h"
#include "DrawableObject.h"

using cimg_library::CImg;
using cimg_library::CImgDisplay;

// Function Declaration
const unsigned int GetUserInputMultiple(const unsigned int& lower, const unsigned int& upper, const unsigned int& multiple);
const unsigned long CalcHexColorFromPixelVal(const float& pixel_value);
float GetUserInputFraction(const float& precision = 0.1);
void GenerateCatmuls(CImg<float>* image, const std::vector<glm::vec3>& all_vecticies);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);

// Globals
Camera g_Camera;
Cursor g_Cursor;
RenderMode g_RenderMode;
unsigned long g_MapIndex;
std::vector<DrawableObject> g_DrawObjs;
bool g_NewCatmul = false;

int main()
{
   std::cout << "Welcome to Image Height Map Generator!" << std::endl;

   // Create a GLFWwindow
   GlfwWindow window("Image Height Map by Christopher McArthur", GlfwWindow::DEFAULT_WIDTH, GlfwWindow::DEFAULT_HEIGHT);
   if (!window()) // Make sure it exists
   {
      return -1;
   }

   // Set the required callback functions
   window.SetKeyCallback(key_callback);
   window.SetMouseButtonCallback(mouse_callback);
   window.SetCursorPosCallback(cursor_callback);

   // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
   glewExperimental = GL_TRUE;
   // Initialize GLEW to setup the OpenGL Function pointers
   if (glewInit() != GLEW_OK)
   {
      std::cout << "Failed to initialize GLEW" << std::endl;
      return -1;
   }

   // Build and compile our shader program
   VertexShader vertexShader("shaders/vertex.shader");
   FragmentShader fragmentShader("shaders/fragment.shader");
   if (!vertexShader() || !fragmentShader()) // make sure they are ready to use
   {
      return -1;
   }

   ShaderLinker* shaderProgram = &ShaderLinker::GetInstance();
   if (!shaderProgram->Link(&vertexShader, &fragmentShader))
   {
      return -1;
   }

   // Constant vectors
   const glm::vec3 center(0.0f, 0.0f, 0.0f);
   const glm::vec3 up(0.0f, 1.0f, 0.0f);
   const glm::vec3 eye(0.0f, 35.0f, 35.0f);

   // Setup global camera
   g_Camera = Camera(center, up, eye);

   // ---------------------------------------------------------------------------------------------
   std::cout << "Processing image....";
   CImg<float> image("assets/depth.bmp");         // load the image
   //CImgDisplay display(image, "Image");           // create window displaying image

   std::vector<glm::vec3> verticies_all;
   std::vector<glm::vec3> colors_all;
   std::vector<GLuint> indinces_all;

   const int img_half_width = image.width() / 2;
   const int img_half_heigth = image.height() / 2;

   for (int x = 0; x < image.width(); x += 1)
   {
      for (int z = 0; z < image.height(); z += 1)
      {
         // verticies
         const float pixel_value = static_cast<float>(*image.data(x, z));
         verticies_all.emplace_back(x - img_half_width, pixel_value, z - img_half_heigth);

         // Color
         const unsigned long colorValue = CalcHexColorFromPixelVal(pixel_value);
         const double red = ((colorValue & 0xff0000) >> 16) / 255.0;
         const double green = ((colorValue & 0x00ff00) >> 8) / 255.0;
         const double blue = (colorValue & 0x0000ff) / 255.0;

         colors_all.emplace_back(red, green, blue);

         // Indicies
         GLint max_column = image.width() - 1;
         GLint max_row = image.height() - 1;
         if (x < max_column - 1 && z < max_row - 1)
         {
            GLint next_index = (int)verticies_all.size();
            GLint pts_per_row = image.height();

            indinces_all.emplace_back(next_index - 1); // this one
            indinces_all.emplace_back(next_index); // next one
            indinces_all.emplace_back(next_index + pts_per_row - 1); // next row

            indinces_all.emplace_back(next_index + pts_per_row - 1); // next row
            indinces_all.emplace_back(next_index); // next one
            indinces_all.emplace_back(next_index + pts_per_row); // across
         }

      }
   }
   std::cout << "  Completed!" << std::endl;
   g_DrawObjs.emplace_back(verticies_all, colors_all, indinces_all);
   // ---------------------------------------------------------------------------------------------

   GenerateCatmuls(&image, verticies_all);

   // Game loop
   while (! ~window)
   {
      // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
      glfwPollEvents();

      if(g_NewCatmul) { GenerateCatmuls(&image, verticies_all); g_NewCatmul = false; } // generate new catmuls on user trigger

      // Clear the colorbuffer
      glClearColor(0.05f, 0.075f, 0.075f, 1.0f); // near black teal
      glClear(GL_COLOR_BUFFER_BIT);

      shaderProgram->SetShaderMat4("view_matrix", g_Camera.GetViewMatrix());

      shaderProgram->SetShaderMat4("projection_matrix", window.GetProjectionMatrix());

      glm::mat4 model_matrix = glm::scale(glm::mat4(), glm::vec3(0.05f));
      shaderProgram->SetShaderMat4("model_matrix", model_matrix);

      // Render
      g_DrawObjs.at(g_MapIndex % g_DrawObjs.size()).Draw(g_RenderMode);

      ++window; // swap buffers
   }

   for (DrawableObject obj: g_DrawObjs) // clean up memory
      obj.Delete();

   return 0;
}

// ------------------------------------------------------------------------------------------------ //
//                                          UTILITY CODE                                          - //
// ------------------------------------------------------------------------------------------------ //

/// copied from another project https://github.com/prince-chrismc/Pandemic/blob/master/Pandemic/Sources/GameEngine.cpp small modifications
const unsigned int GetUserInputMultiple(const unsigned int& lower, const unsigned int& upper, const unsigned int& multiple)
{
   uint16_t selection = 0;
   do
   {
      std::cout << "Selcetion: ";
      std::string input;
      std::getline(std::cin, input);
      std::stringstream ss(input);
      ss >> selection;

      if (selection < lower || selection > upper || selection % multiple != 0)
         std::cout << "Invalid option. Please Try again..." << std::endl;

   } while (selection < lower || selection > upper || selection % multiple != 0);

   return selection;
}

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

void GenerateCatmuls(CImg<float>* image, const std::vector<glm::vec3>& all_vecticies)
{
   const int img_half_width = image->width() / 2;
   const int img_half_heigth = image->height() / 2;

   std::vector<glm::vec3> verticies_stripped(all_vecticies);

   std::cout << std::endl << "Please Select a skip size: (multiple of 5) recommended: 20" << std::endl;
   int skip_size = GetUserInputMultiple(5, 65, 5);

   std::cout << std::endl << "Please input a step size: (multiple of 0.1) recommended: 0.2" << std::endl;
   float step_size = GetUserInputFraction();

   std::cout << "Processing splines....";
   verticies_stripped.erase(std::remove_if(verticies_stripped.begin(), verticies_stripped.end(), [=](glm::vec3 vec){ return (static_cast<int>(vec.x + img_half_width) % skip_size != 0) || (static_cast<int>(vec.z + img_half_heigth) % skip_size == 0); }), verticies_stripped.end()); // removes all points which dont match skip_size

   const glm::mat4 basis_mat(-0.5,  1.5, -1.5,  0.5,
                        1.0, -2.5,  2.0, -0.5,
                       -0.5,  0.0,  0.5,  0.0,
                        0.0,  1.0,  0.0,  0.0);

   std::vector<glm::vec3> verticies_x_catmul;
   std::vector<glm::vec3> colors_x_catmul;
   std::vector<GLuint> indicies_x_catmul;

   for (size_t index = 1; index < verticies_stripped.size() - 2; index += 1)                       // for all reamining points
   {
      if(verticies_stripped.at(index - 1).x != verticies_stripped.at(index).x) continue;
      if(verticies_stripped.at(index).x != verticies_stripped.at(index + 1).x) continue;
      if(verticies_stripped.at(index + 2).x != verticies_stripped.at(index).x) continue;           // ignoring edges

      for (float step = 0.0; step < 1.0; step += step_size)                                        // calc each step
      {
         glm::vec4 u_vec(std::pow(step, 3), std::pow(step, 2), step, 1.0f);
         glm::mat4x3 control_mat(verticies_stripped.at(index - 1),
                                 verticies_stripped.at(index),
                                 verticies_stripped.at(index + 1),
                                 verticies_stripped.at(index + 2));                                // along X axis

         // verticies
         verticies_x_catmul.emplace_back(control_mat * basis_mat * u_vec);                         // save new point

         // Color
         const unsigned long colorValue = CalcHexColorFromPixelVal(verticies_x_catmul.back().y);   // calc and save color
         colors_x_catmul.emplace_back(((colorValue & 0xff0000) >> 16) / 255.0, ((colorValue & 0x00ff00) >> 8) / 255.0, (colorValue & 0x0000ff) / 255.0);
      }
   }

   GLuint points_per_row = 0;                                                                      // this is the long (lazy way) to find length ... i couldnt figure out equation
   for (size_t index = 0; index < verticies_x_catmul.size(); index += 1)                           // this is the long (lazy way) to find length ... i couldnt figure out equation
   {                                                                                               // this is the long (lazy way) to find length ... i couldnt figure out equation
      if (verticies_x_catmul.at(0).x + skip_size == verticies_x_catmul.at(index).x)                // this is the long (lazy way) to find length ... i couldnt figure out equation
      {                                                                                            // this is the long (lazy way) to find length ... i couldnt figure out equation
         points_per_row = (unsigned int)index + 1;                                                 // this is the long (lazy way) to find length ... i couldnt figure out equation
         break;                                                                                    // this is the long (lazy way) to find length ... i couldnt figure out equation
      }                                                                                            // this is the long (lazy way) to find length ... i couldnt figure out equation
   }                                                                                               // this is the long (lazy way) to find length ... i couldnt figure out equation

   // Indicies
   for (GLuint index = 0; index < verticies_x_catmul.size() - 1; index += 1)                       // for each point
   {
      if( index + points_per_row + 1 >= verticies_x_catmul.size()) continue;
      if (verticies_x_catmul.at(index).x != verticies_x_catmul.at(index + 1).x) continue;          // ignoring edges
      if (verticies_x_catmul.at(index + points_per_row).x != verticies_x_catmul.at(index + points_per_row + 1).x) continue;

      indicies_x_catmul.emplace_back(index);
      indicies_x_catmul.emplace_back(index + 1);
      indicies_x_catmul.emplace_back(index + points_per_row);

      indicies_x_catmul.emplace_back(index + 1);
      indicies_x_catmul.emplace_back(index + points_per_row);
      indicies_x_catmul.emplace_back(index + points_per_row + 1);                                  // calc and save triangles (adj + across)
   }

   g_DrawObjs.emplace_back(verticies_x_catmul, colors_x_catmul, indicies_x_catmul);                // save to draw later

   // -----------------------------------------------------------------------------------------------------------------------------------------------

   std::vector<glm::vec3> verticies_passed(verticies_x_catmul);                                    // copy prev points

   std::vector<glm::vec3> verticies_xz_catmul;
   std::vector<glm::vec3> colors_xz_catmul;
   std::vector<GLuint> indicies_xz_catmul;

   points_per_row -= 1;                                                                            // adjust for element access

   for (size_t index = 0; index < verticies_passed.size() - 1; index += 1)                         // for each point
   {
      if ((long)index - (long)points_per_row < 0) continue;
      if (index + points_per_row > verticies_passed.size() - 1) continue;
      if (index + 2*points_per_row > verticies_passed.size() - 1) continue;

      if (verticies_passed.at(index).z != verticies_passed.at(index - points_per_row).z) continue;
      if (verticies_passed.at(index).z != verticies_passed.at(index + points_per_row).z) continue; // ignoring edge cases
      if (verticies_passed.at(index).z != verticies_passed.at(index + 2*points_per_row).z) continue;

      for (float step = 0.0; step < 1.0; step += step_size)
      {
         glm::vec4 u_vec(std::pow(step, 3), std::pow(step, 2), step, 1.0f);
         glm::mat4x3 control_mat(verticies_passed.at(index - points_per_row),
                                 verticies_passed.at(index),
                                 verticies_passed.at(index + points_per_row),
                                 verticies_passed.at(index + 2*points_per_row));                   // along Z axis

         // verticies
         verticies_xz_catmul.emplace_back(control_mat * basis_mat * u_vec);                        // calc and store new point
      }
   }

   // sort back to cartisian plan order -- Ex: { {0,0}, {0,1}, {1,0}, {1,1} }
   std::stable_sort(verticies_xz_catmul.begin(), verticies_xz_catmul.end(), [](glm::vec3 a, glm::vec3 b) { return (a.x < b.x); });                // why stable_sort and not sort...
   std::stable_sort(verticies_xz_catmul.begin(), verticies_xz_catmul.end(), [](glm::vec3 a, glm::vec3 b) { return (a.x < b.x) && (a.z < b.z); }); // https://stackoverflow.com/a/7215833/8480874

   // Color
   for (glm::vec3 coord : verticies_xz_catmul)                                                     // for each point
   {
      const unsigned long colorValue = CalcHexColorFromPixelVal(coord.y);                          // calc color and store
      colors_xz_catmul.emplace_back(((colorValue & 0xff0000) >> 16) / 255.0, ((colorValue & 0x00ff00) >> 8) / 255.0, (colorValue & 0x0000ff) / 255.0);
   }

   points_per_row = 0;                                                                             // this is the long (lazy way) to find length ... i couldnt figure out equation
   for (GLuint index = 0; index < verticies_xz_catmul.size(); index += 1)                          // this is the long (lazy way) to find length ... i couldnt figure out equation
   {                                                                                               // this is the long (lazy way) to find length ... i couldnt figure out equation
      if (verticies_xz_catmul.at(0).x + skip_size == verticies_xz_catmul.at(index).x)              // this is the long (lazy way) to find length ... i couldnt figure out equation
      {                                                                                            // this is the long (lazy way) to find length ... i couldnt figure out equation
         points_per_row = (unsigned int)index + 1;                                                 // this is the long (lazy way) to find length ... i couldnt figure out equation
         break;                                                                                    // this is the long (lazy way) to find length ... i couldnt figure out equation
      }                                                                                            // this is the long (lazy way) to find length ... i couldnt figure out equation
   }                                                                                               // this is the long (lazy way) to find length ... i couldnt figure out equation

   // Indicies
   for (GLuint index = 0; index < verticies_xz_catmul.size() - 1; index += 1)                      // for every single damn point.............................................
   {
      if (index + points_per_row + 1 >= verticies_xz_catmul.size()) continue;
      if (verticies_xz_catmul.at(index).x != verticies_xz_catmul.at(index + 1).x) continue;        // ignore edges
      if (verticies_xz_catmul.at(index + points_per_row).x != verticies_xz_catmul.at(index + points_per_row + 1).x) continue;

      indicies_xz_catmul.emplace_back(index);
      indicies_xz_catmul.emplace_back(index + 1);
      indicies_xz_catmul.emplace_back(index + points_per_row);

      indicies_xz_catmul.emplace_back(index + 1);
      indicies_xz_catmul.emplace_back(index + points_per_row);
      indicies_xz_catmul.emplace_back(index + points_per_row + 1);                                 // calc and save triangles (adj + across)
   }

   g_DrawObjs.emplace_back(verticies_xz_catmul, colors_xz_catmul, indicies_xz_catmul);             // store obj to be drawn
   std::cout << "  Completed!" << std::endl;
}

// ------------------------------------------------------------------------------------------------ //
//                                      CALLBACK FUNCTIONS                                        - //
// ------------------------------------------------------------------------------------------------ //

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
   // we are only concerned about key presses not releases
   if (action != GLFW_PRESS)
      return;

   // lets print the key in human readable if possible
   const char* keyname = glfwGetKeyName(key, scancode);
   if (keyname)
      std::cout << keyname << std::endl;
   else
      std::cout << key << std::endl;

   switch (key)
   {
      // windows close
   case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(window, GLFW_TRUE);
      break;

      // move camera
   case GLFW_KEY_UP:
      g_Camera.AdjustRotateXAxis(5.0);
      break;
   case GLFW_KEY_DOWN:
      g_Camera.AdjustRotateXAxis(-5.0);
      break;
   case GLFW_KEY_LEFT:
      g_Camera.AdjustRotateZAxis(5.0);
      break;
   case GLFW_KEY_RIGHT:
      g_Camera.AdjustRotateZAxis(-5.0);
      break;
   case GLFW_KEY_W:
      g_Camera.AdjustTranslateZAxis(5.0);
      break;
   case GLFW_KEY_S:
      g_Camera.AdjustTranslateZAxis(-5.0);
      break;
   case GLFW_KEY_D:
      g_Camera.AdjustTranslateXAxis(-5.0);
      break;
   case GLFW_KEY_A:
      g_Camera.AdjustTranslateXAxis(5.0);
      break;

      // reset everything
   case GLFW_KEY_BACKSPACE:
      g_NewCatmul = true;
   case GLFW_KEY_HOME:
      g_Camera.ResetCameraPos();
      break;

      // render mode
   case GLFW_KEY_P:
      g_RenderMode = RenderMode::POINTS;
      break;
   case GLFW_KEY_T:
      g_RenderMode = RenderMode::TRIANGLES;
      break;

   case GLFW_KEY_ENTER:
   case GLFW_KEY_SPACE:
      g_MapIndex += 1;

   default:
      return;
   };
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
   double lastX, lastY;
   if (button == GLFW_MOUSE_BUTTON_LEFT) {
      if (action == GLFW_PRESS)
      {
         std::cout << "allow zooming" << std::endl;
         glfwGetCursorPos(window, &lastX, &lastY);
         g_Cursor.SetLastY((float)lastY);
         g_Cursor.ToggleLeftButton();
      }
      else
      {
         std::cout << "disable zooming" << std::endl;
         g_Cursor.ToggleLeftButton();

      }
   }
   else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
      if (action == GLFW_PRESS)
      {
         std::cout << "allow rotate y" << std::endl;
         glfwGetCursorPos(window, &lastX, &lastY);
         g_Cursor.SetLastY((float)lastY);
         g_Cursor.ToggleRightButton();
      }
      else
      {
         std::cout << "disable rotate y" << std::endl;
         g_Cursor.ToggleRightButton();

      }
   }
}

void cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
   if (g_Cursor.IsLeftActive())
   {
      float yoffset = static_cast<float>(g_Cursor.GetLastY() - ypos);
      g_Cursor.SetLastY((float)ypos);
      yoffset *= Cursor::Sensitivity;

      g_Camera.AdjustScalar(yoffset);
   }
   if (g_Cursor.IsRightActive())
   {
      float yoffset = static_cast<float>(g_Cursor.GetLastY() - ypos);
      g_Cursor.SetLastY((float)ypos);
      yoffset *= Cursor::Sensitivity;

      g_Camera.AdjustRotateYAxis(yoffset);
   }
}
