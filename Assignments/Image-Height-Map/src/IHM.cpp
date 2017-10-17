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
const unsigned int GetUserInputMultiple(const unsigned int& lower, const unsigned int& upper, const unsigned int& multiple);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);

// Globals
Camera g_Camera;
Cursor g_Cursor;
RenderMode g_RenderMode;
unsigned long g_MapIndex;

int main()
{
   std::cout << "Welcome to Image Height Map Generator!" << std::endl;

   std::cout << std::endl << "Please Select a skip size: (multiple of 5) recommended: 20" << std::endl;
   int skip_size = GetUserInputMultiple(5, 65, 5);

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

   //glEnable(GL_DEPTH_TEST);
   //glDepthFunc(GL_LESS);

   // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
   glewExperimental = GL_TRUE;
   // Initialize GLEW to setup the OpenGL Function pointers
   if (glewInit() != GLEW_OK)
   {
      std::cout << "Failed to initialize GLEW" << std::endl;
      return -1;
   }

   //glEnable(GL_DEPTH_TEST);
   //glDepthFunc(GL_GEQUAL);

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
   const glm::vec3 eye(0.0f, 35.0f, 35.0f);

   // Setup global camera
   g_Camera = Camera(center, up, eye);


   // ---------------------------------------------------------------------------------------------
   std::cout << "Processing image....";
   CImg<float> image("assets/depth.bmp");         // load the image
   CImgDisplay display(image, "Image");           // create window displaying image

   std::vector<glm::vec3> verticies_all;
   std::vector<glm::vec3> colors_all;
   std::vector<GLuint> indinces_all;

   std::vector<glm::vec3> verticies_skip;
   std::vector<glm::vec3> colors_skip;
   std::vector<GLuint> indinces_skip;

   std::vector<glm::vec3> verticies_skip_skip;
   std::vector<glm::vec3> colors_skip_skip;
   std::vector<GLuint> indinces_skip_skip;

   int img_half_width = image.width() / 2;
   int img_half_heigth = image.height() / 2;

   for (int x = (0 - img_half_width); x < img_half_width; x += 1)
   {
      for (int z = (0 - img_half_heigth); z < img_half_heigth; z += 1)
      {
         // verticies
         double pixel_value = static_cast<float>(*image.data(x + img_half_width, z + img_half_heigth));
         verticies_all.emplace_back(x, pixel_value, z);
         if(x % skip_size == 0)
         {
            verticies_skip.emplace_back(x, pixel_value, z);
            if (z % skip_size == 0)
            {
               verticies_skip_skip.emplace_back(x, pixel_value, z);
            }
         }

         // Color
         unsigned long colorValue = static_cast<unsigned long>(std::floor(std::pow(pixel_value, 2.0))); // blue to green
         colors_all.emplace_back(((colorValue & 0xff0000) >> 16)/255.0, ((colorValue & 0x00ff00) >> 8)/255.0, (colorValue & 0x0000ff)/255.0);
         if (x % skip_size == 0)
         {
            colors_skip.emplace_back(((colorValue & 0xff0000) >> 16) / 255.0, ((colorValue & 0x00ff00) >> 8) / 255.0, (colorValue & 0x0000ff) / 255.0);
            if (z % skip_size == 0)
            {
               colors_skip_skip.emplace_back(((colorValue & 0xff0000) >> 16) / 255.0, ((colorValue & 0x00ff00) >> 8) / 255.0, (colorValue & 0x0000ff) / 255.0);
            }
         }

         // Indicies
         GLint max_column = img_half_width-1;
         GLint max_row = img_half_heigth-1;
         if (x < max_column - 1 && z < max_row - 1)
         {
            GLint next_index = verticies_all.size();
            GLint pts_per_row = image.height();

            indinces_all.emplace_back(next_index - 1); // this one
            indinces_all.emplace_back(next_index); // next one
            indinces_all.emplace_back(next_index + pts_per_row - 1); // next row

            indinces_all.emplace_back(next_index + pts_per_row - 1); // next row
            indinces_all.emplace_back(next_index); // next one
            indinces_all.emplace_back(next_index + pts_per_row); // across
         }

         if (x % skip_size == 0  && x < (max_column - skip_size) && z < (max_row - 1))
         {
            GLint next_index = verticies_skip.size();
            GLint pts_per_row = image.height();

            indinces_skip.emplace_back(next_index - 1); // this one
            indinces_skip.emplace_back(next_index); // next one
            indinces_skip.emplace_back(next_index + pts_per_row - 1); // next row

            indinces_skip.emplace_back(next_index + pts_per_row - 1); // next row
            indinces_skip.emplace_back(next_index); // next one
            indinces_skip.emplace_back(next_index + pts_per_row); // across
         }

         if (z % skip_size == 0 && x % skip_size == 0 && x < (max_column - skip_size) && z < (max_row - skip_size))
         {
            GLint next_index = verticies_skip_skip.size();
            GLint pts_per_row = image.height() / skip_size + 1;

            indinces_skip_skip.emplace_back(next_index - 1); // this one
            indinces_skip_skip.emplace_back(next_index); // next one
            indinces_skip_skip.emplace_back(next_index + pts_per_row - 1); // next row

            indinces_skip_skip.emplace_back(next_index + pts_per_row - 1); // next row
            indinces_skip_skip.emplace_back(next_index); // next one
            indinces_skip_skip.emplace_back(next_index + pts_per_row); // across
         }
      }
   }
   std::cout << "  Completed!" << std::endl;

   // All points --------------------------------------------------------------------------------------------------
   GLuint VAO_all_pts, VBO_all_pts, VBO_all_color, IBO_all;
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

   glGenBuffers(1, &IBO_all);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_all);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indinces_all.size() * sizeof(GLuint), &indinces_all.front(), GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   glBindVertexArray(0);

   // Skip Points ----------------------------------------------------------------------------------------------------
   GLuint VAO_skip_pts, VBO_skip_pts, VBO_skip_color, IBO_skip;
   glGenVertexArrays(1, &VAO_skip_pts);
   glBindVertexArray(VAO_skip_pts);

   glGenBuffers(1, &VBO_skip_pts);
   glBindBuffer(GL_ARRAY_BUFFER, VBO_skip_pts);
   glBufferData(GL_ARRAY_BUFFER, verticies_skip.size() * sizeof(glm::vec3), &verticies_skip.front(), GL_STATIC_DRAW);
   glVertexAttribPointer(PositonIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(PositonIndex);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glGenBuffers(1, &VBO_skip_color);
   glBindBuffer(GL_ARRAY_BUFFER, VBO_skip_color);
   glBufferData(GL_ARRAY_BUFFER, colors_skip.size() * sizeof(glm::vec3), &colors_skip.front(), GL_STATIC_DRAW);
   glVertexAttribPointer(ColorIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(ColorIndex);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glGenBuffers(1, &IBO_skip);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_skip);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indinces_skip.size() * sizeof(GLuint), &indinces_skip.front(), GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   glBindVertexArray(0);

   // Skip X Skip Points ----------------------------------------------------------------------------------------------------
   GLuint VAO_skip_skip_pts, VBO_skip_skip_pts, VBO_skip_skip_color, IBO_skip_skip;
   glGenVertexArrays(1, &VAO_skip_skip_pts);
   glBindVertexArray(VAO_skip_skip_pts);

   glGenBuffers(1, &VBO_skip_skip_pts);
   glBindBuffer(GL_ARRAY_BUFFER, VBO_skip_skip_pts);
   glBufferData(GL_ARRAY_BUFFER, verticies_skip_skip.size() * sizeof(glm::vec3), &verticies_skip_skip.front(), GL_STATIC_DRAW);
   glVertexAttribPointer(PositonIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(PositonIndex);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glGenBuffers(1, &VBO_skip_skip_color);
   glBindBuffer(GL_ARRAY_BUFFER, VBO_skip_skip_color);
   glBufferData(GL_ARRAY_BUFFER, colors_skip_skip.size() * sizeof(glm::vec3), &colors_skip_skip.front(), GL_STATIC_DRAW);
   glVertexAttribPointer(ColorIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(ColorIndex);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glGenBuffers(1, &IBO_skip_skip);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_skip_skip);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indinces_skip_skip.size() * sizeof(GLuint), &indinces_skip_skip.front(), GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   glBindVertexArray(0);
   // ---------------------------------------------------------------------------------------------


   // Game loop
   while (! ~window)
   {
      // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
      glfwPollEvents();

      // Render
      // Clear the colorbuffer
      glClearColor(0.05f, 0.075f, 0.075f, 1.0f); // near black teal
      glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/);

      shaderProgram->SetShaderMat4("view_matrix", g_Camera.GetViewMatrix());

      shaderProgram->SetShaderMat4("projection_matrix", window.GetProjectionMatrix());

      glm::mat4 model_matrix = glm::scale(glm::mat4(), glm::vec3(0.05f));
      shaderProgram->SetShaderMat4("model_matrix", model_matrix);

      switch (g_MapIndex % 3)
      {
      case 0:
         // All Points
         switch (g_RenderMode)
         {
         case RenderMode::POINTS:
            glBindVertexArray(VAO_all_pts);
            glDrawArrays(GL_POINTS, 0, (GLsizei)verticies_all.size());
            glBindVertexArray(0);
            break;

         case RenderMode::TRIANGLES:
            glBindVertexArray(VAO_all_pts);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_all);
            glDrawElements(GL_TRIANGLES, indinces_all.size(), GL_UNSIGNED_INT, NULL);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            break;
         }
         break;
         case 1:
         // Skip Points
         switch (g_RenderMode)
         {
         case RenderMode::POINTS:
            glBindVertexArray(VAO_skip_pts);
            glDrawArrays((GLuint)g_RenderMode, 0, (GLsizei)verticies_skip.size());
            glBindVertexArray(0);
            break;

         case RenderMode::TRIANGLES:
            glBindVertexArray(VAO_skip_pts);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_skip);
            glDrawElements(GL_TRIANGLES, indinces_skip.size(), GL_UNSIGNED_INT, NULL);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            break;
         }
         break;
         case 2:
         // Skip X Skip Points
         switch (g_RenderMode)
         {
         case RenderMode::POINTS:
            glBindVertexArray(VAO_skip_skip_pts);
            glDrawArrays((GLuint)g_RenderMode, 0, (GLsizei)verticies_skip_skip.size());
            glBindVertexArray(0);
            break;

         case RenderMode::TRIANGLES:
            glBindVertexArray(VAO_skip_skip_pts);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_skip_skip);
            glDrawElements(GL_TRIANGLES, indinces_skip_skip.size(), GL_UNSIGNED_INT, NULL);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            break;
         }
         break;
         default:
         break;
      }

      ++window; // swap buffers
   }

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
   case GLFW_KEY_HOME:
   case GLFW_KEY_BACKSPACE:
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
