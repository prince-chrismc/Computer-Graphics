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

#include "glm/gtc/matrix_transform.hpp" //glm::lookAt, scale, etc...
#include "GL/glew.h"                    // include GL Extension Wrangler
#include "CImg.h"

#include "GlfwWindow.h"
#include "Shader.h"
#include "Camera.h"
#include "Cursor.h"

using cimg_library::CImg;
using cimg_library::CImgDisplay;

//enums
enum class RenderMode { POINTS = GL_POINTS, LINES = GL_LINES, TRIANGLES = GL_TRIANGLES };

// Function Declaration
//const unsigned int GetUserInput(const unsigned int& lower, const unsigned int& upper);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);

// Globals
Camera g_Camera;
Cursor g_Cursor;
RenderMode g_RenderMode;

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

   GLuint PositonIndex = shaderProgram->GetAttributeLocation("position");
   GLuint ColorIndex = shaderProgram->GetAttributeLocation("color");

   // Constant vectors
   const glm::vec3 center(0.0f, 0.0f, 0.0f);
   const glm::vec3 up(0.0f, 1.0f, 0.0f);
   const glm::vec3 eye(0.0f, 50.0f, -50.0f);

   // Setup global camera
   g_Camera = Camera(center, up, eye);


   // ---------------------------------------------------------------------------------------------
   std::cout << "Processing image....";
   CImg<float> image("assets/depth.bmp");         // load the image
   CImgDisplay display(image, "Image");           // create window displaying image

   int x = (0 - image.width()), z = (0 - image.height());
   std::vector<glm::vec3> verticies_all;
   std::vector<glm::vec3> colors_all;
   //float max_height = 0.0f; // test code
   //float min_height = 255.0f; // test code

   for (CImg<float>::iterator it = image.begin(); it < image.end(); ++it)
   {
      //(max_height < *it) ? max_height = *it : void(); // test code
      //(min_height > *it) ? min_height = *it : void(); // test code
      verticies_all.emplace_back(glm::vec3(x++, *it, z));

      int colorValue = std::floor(1.25*std::pow(*it, 3.0)); // blue to pink
      //int colorValue = std::floor(1.5*std::pow(*it, 2.0)); // blue to green
      colors_all.emplace_back(glm::vec3(((colorValue & 0xff0000) >> 16)/255.0, ((colorValue & 0x00ff00) >> 8)/255.0, (colorValue & 0x0000ff)/255.0)); // https://stackoverflow.com/a/15693516/8480874

      if(x == image.width()) {x = (0 - image.width() ); z += 1; }
   }
   std::cout << "  Completed!" <<std::endl;
   //std::cout << "The Max height is: " << max_height << std::endl; // test code - was 252
   //std::cout << "The Min height is: " << min_height << std::endl; // test code - was 67

   GLuint VAO_all_pts, VBO_all_pts, VBO_all_color;
   glGenVertexArrays(1, &VAO_all_pts);
   glBindVertexArray(VAO_all_pts);

   glGenBuffers(1, &VBO_all_pts);
   glBindBuffer(GL_ARRAY_BUFFER, VBO_all_pts);
   glBufferData(GL_ARRAY_BUFFER, verticies_all.size() * sizeof(glm::vec3), &verticies_all.front(), GL_STATIC_DRAW);
   glVertexAttribPointer(PositonIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(PositonIndex);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glGenBuffers(1, &VBO_all_color);
   glBindBuffer(GL_ARRAY_BUFFER, VBO_all_color);
   glBufferData(GL_ARRAY_BUFFER, colors_all.size() * sizeof(glm::vec3), &colors_all.front(), GL_STATIC_DRAW);
   glVertexAttribPointer(ColorIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(ColorIndex);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glBindVertexArray(0);
   // ---------------------------------------------------------------------------------------------


   // Game loop
   while (! ~window)
   {
      // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
      glfwPollEvents();

      // Render
      // Clear the colorbuffer
      glClearColor(0.05f, 0.075f, 0.075f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      shaderProgram->SetShaderMat4("view_matrix", g_Camera.GetViewMatrix());

      shaderProgram->SetShaderMat4("projection_matrix", window.GetProjectionMatrix());

      glm::mat4 model_matrix = glm::scale(glm::mat4(), glm::vec3(0.05f));
      shaderProgram->SetShaderMat4("model_matrix", model_matrix);

      //shaderProgram->SetShaderInt("object_color", (GLint)ObjectColors::GREY);
      glBindVertexArray(VAO_all_pts);
      glDrawArrays(GL_POINTS, 0, (GLsizei)verticies_all.size());
      glBindVertexArray(0);

      ++window; // swap buffers
   }

   return 0;
}

// ------------------------------------------------------------------------------------------------ //
//                                          UTILITY CODE                                          - //
// ------------------------------------------------------------------------------------------------ //

/// copied from another project https://github.com/prince-chrismc/Pandemic/blob/master/Pandemic/Sources/GameEngine.cpp small modifications
//const unsigned int GetUserInput(const unsigned int& lower, const unsigned int& upper)
//{
//   uint16_t selection = 0;
//   do
//   {
//      std::cout << "Selcetion: ";
//      std::string input;
//      std::getline(std::cin, input);
//      std::stringstream ss(input);
//      ss >> selection;
//
//      if (selection < lower || selection > upper)
//         std::cout << "Invalid option. Please Try again..." << std::endl;
//
//   } while (selection < lower || selection > upper || selection % 2 != 1);
//
//   return selection;
//}

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
   const char* keyname = glfwGetKeyName(key, scancode); // function found at https://github.com/glfw/glfw/pull/117
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
      g_Camera.AdjustTranslateXAxis(5.0);
      break;
   case GLFW_KEY_S:
      g_Camera.AdjustTranslateXAxis(-5.0);
      break;
   case GLFW_KEY_D:
      g_Camera.AdjustTranslateZAxis(5.0);
      break;
   case GLFW_KEY_A:
      g_Camera.AdjustTranslateZAxis(-5.0);
      break;

      // reset everything
   case GLFW_KEY_HOME:
   case GLFW_KEY_BACKSPACE:
      g_Camera.ResetCameraPos();
      break;

      // render mode
   case GLFW_KEY_P:
      g_RenderMode = RenderMode::POINTS;
      break;
   case GLFW_KEY_L:
      g_RenderMode = RenderMode::LINES;
      break;
   case GLFW_KEY_T:
      g_RenderMode = RenderMode::TRIANGLES;
      break;

   default:
      return;
   };
}

// inspiration https://stackoverflow.com/questions/37194845/using-glfw-to-capture-mouse-dragging-c for below
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

// inspiration https://learnopengl.com/#!Getting-started/Camera
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
