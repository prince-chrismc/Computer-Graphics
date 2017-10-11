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

#include "GL/glew.h"       // include GL Extension Wrangler
#include "glm/gtc/matrix_transform.hpp" //glm::lookAt
#include "CImg.h"

#include "GlfwWindow.h"
#include "Shader.h"

using cimg_library::CImg;
using cimg_library::CImgDisplay;

enum class ObjectColors { RED, GREEN, BLUE, GREY, YELLOW, TEAL };


int main()
{
   std::cout << "Welcome to Image Height Map Generator!" << std::endl;

   // Create a GLFWwindow
   GlfwWindow window("Image Height Map by Christopher McArthur", GlfwWindow::DEFAULT_WIDTH, GlfwWindow::DEFAULT_HEIGHT);
   if (!window()) // Make sure it exists
   {
      return -1;
   }

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
   const glm::vec3 eye(0.0f, 50.0f, -50.0f);


   // ---------------------------------------------------------------------------------------------
   std::cout << "Processing image....";
   CImg<float> image("assets/depth.bmp"); // load the image
   CImgDisplay display(image, "Image");           // create window displaying image

   int x = (0 - image.width()/2), z = (0 - image.height() / 2);
   std::vector<glm::vec3> verticies_all;

   for (CImg<float>::iterator it = image.begin(); it < image.end(); ++it)
   {
      verticies_all.emplace_back(glm::vec3(x++, *it, z));
      if(x == image.width()) {x = (0 - image.width() / 2); z += 1; }
   }
   std::cout << "  Completed!" <<std::endl;

   GLuint VAO_all_pts, VBO_all_pts;
   glGenVertexArrays(1, &VAO_all_pts);
   glBindVertexArray(VAO_all_pts);

   glGenBuffers(1, &VBO_all_pts);
   glBindBuffer(GL_ARRAY_BUFFER, VBO_all_pts);
   glBufferData(GL_ARRAY_BUFFER, verticies_all.size() * sizeof(glm::vec3), &verticies_all.front(), GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(0);
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

      glm::mat4 view_matrix;
      view_matrix = glm::lookAt(eye, center, up);
      shaderProgram->SetShaderMat4("view_matrix", view_matrix);

      shaderProgram->SetShaderMat4("projection_matrix", window.GetProjectionMatrix());

      glm::mat4 model_matrix = glm::scale(glm::mat4(), glm::vec3(0.05f));
      shaderProgram->SetShaderMat4("model_matrix", model_matrix);

      shaderProgram->SetShaderInt("object_color", (GLint)ObjectColors::GREY);
      glBindVertexArray(VAO_all_pts);
      glDrawArrays(GL_POINTS, 0, (GLsizei)verticies_all.size());
      glBindVertexArray(0);

      ++window; // swap buffers
   }

   return 0;
}
