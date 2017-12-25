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

// Template.cpp : Defines the entry point for the console application.
//

#include <string>
#include <iostream>

#include "GL/glew.h"                            // include GL Extension Wrangler
#include "glm/gtc/matrix_transform.hpp"         //glm::lookAt

#include <ft2build.h>
#include FT_FREETYPE_H

#include "GlfwWindow.h"
#include "Shader.h"

int main()
{
   std::cout << "Welcome to Template!" << std::endl;

   // Create a GLFWwindow
   std::shared_ptr<GlfwWindow> window = GlfwWindowFactory::GetInstance()->CreateNewWindow("Template by <Author>", GlfwWindow::DEFAULT_WIDTH, GlfwWindow::DEFAULT_HEIGHT);
   if (!window->IsValid())                      // Make sure it exists
   {
      return -1;
   }

   // Set the required callback functions
   /*
   * window->SetKeyCallback(key_callback);
   * window->SetMouseButtonCallback(mouse_callback);
   * window->SetCursorPosCallback(cursor_callback);
   */

   if (glewInit() != GLEW_OK)                   // Initialize GLEW to setup the OpenGL Function pointers
   {
      std::cout << "Failed to initialize GLEW" << std::endl;
      std::cout << "Press 'enter' to exit." << std::endl;
      std::getline(std::cin, std::string());
      return -1;
   }

   // Build and compile our shader program
   VertexShader vertexShader("shaders/vertex.shader");
   FragmentShader fragmentShader("shaders/fragment.shader");
   if (!vertexShader() || !fragmentShader())    // make sure they are ready to use
   {
      std::cout << "Press 'enter' to exit." << std::endl;
      std::getline(std::cin, std::string());
      return -1;
   }

   auto shaderProgram = ShaderLinker::GetInstance();
   if (!shaderProgram->Link(&vertexShader, &fragmentShader))
   {
      std::cout << "Press 'enter' to exit." << std::endl;
      std::getline(std::cin, std::string());
      return -1;
   }

   FT_Library ft;
   if (FT_Init_FreeType(&ft))
      std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

   // Sample code for using freetype
   /*
   FT_Face face;
   if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
      std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

   FT_Set_Pixel_Sizes(face, 0, 48);

   if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
   */

   // Constant vectors
   const glm::vec3 center(0.0f, 0.0f, 0.0f);
   const glm::vec3 up(0.0f, 0.0f, 1.0f);
   const glm::vec3 eye(0.0f, -5.0f, 3.0f);

   // Game loop
   while (!window->ShouldClose())
   {
      // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
      GlfwWindow::TriggerCallbacks();

      // Render
      // Clear the colorbuffer
      glClearColor(0.05f, 0.075f, 0.075f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glm::mat4 view_matrix;
      view_matrix = glm::lookAt(eye, center, up);
      shaderProgram->SetShaderMat4("view_matrix", view_matrix);

      shaderProgram->SetShaderMat4("projection_matrix", window->GetProjectionMatrix());

      window->NextBuffer(); // swap buffers
   }

   return 0;
}
