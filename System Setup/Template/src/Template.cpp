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
#include <fstream>

#include "GL/glew.h"       // include GL Extension Wrangler

#include "GlfwWindow.h"

int main()
{
   std::cout << "Welcome to Template!" << std::endl;

   // Create a GLFWwindow
   GlfwWindow window(GlfwWindow::DEFAULT_WIDTH, GlfwWindow::DEFAULT_HEIGHT);
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
   // Vertex shader

   // Read the Vertex Shader code from the file
   std::string vertex_shader_path = "shaders/vertex.shader";
   std::string VertexShaderCode;
   std::ifstream VertexShaderStream(vertex_shader_path, std::ios::in);

   if (VertexShaderStream.is_open()) {
      std::string Line = "";
      while (std::getline(VertexShaderStream, Line))
         VertexShaderCode += "\n" + Line;
      VertexShaderStream.close();
   }
   else {
      printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
      getchar();
      exit(-1);
   }

   // Read the Fragment Shader code from the file
   std::string fragment_shader_path = "shaders/fragment.shader";
   std::string FragmentShaderCode;
   std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);

   if (FragmentShaderStream.is_open()) {
      std::string Line = "";
      while (getline(FragmentShaderStream, Line))
         FragmentShaderCode += "\n" + Line;
      FragmentShaderStream.close();
   }
   else {
      printf("Impossible to open %s. Are you in the right directory?\n", fragment_shader_path.c_str());
      getchar();
      exit(-1);
   }

   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   char const * VertexSourcePointer = VertexShaderCode.c_str();
   glShaderSource(vertexShader, 1, &VertexSourcePointer, NULL);
   glCompileShader(vertexShader);
   // Check for compile time errors
   GLint success;
   GLchar infoLog[512];
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
   }
   // Fragment shader
   GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   char const * FragmentSourcePointer = FragmentShaderCode.c_str();
   glShaderSource(fragmentShader, 1, &FragmentSourcePointer, NULL);
   glCompileShader(fragmentShader);
   // Check for compile time errors
   glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
   }
   // Link shaders
   GLuint shaderProgram = glCreateProgram();
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   glLinkProgram(shaderProgram);
   // Check for linking errors
   glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
   if (!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
   }
   glDeleteShader(vertexShader); //free up memory
   glDeleteShader(fragmentShader);

   glUseProgram(shaderProgram);






   return 0;
}

