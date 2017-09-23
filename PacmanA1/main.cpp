//example skeleton code
//modified from http://learnopengl.com/

#include "GL/glew.h"      // include GL Extension Wrangler
#include "GLFW/glfw3.h"   // include GLFW helper library
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "objloader.hpp"  //include the object loader

using namespace std;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

glm::vec3 camera_position;
glm::vec3 triangle_scale;
glm::mat4 projection_matrix;

// Constant vectors
const glm::vec3 center(0.0f, 0.0f, 0.0f);
const glm::vec3 up(0.0f, 1.0f, 0.0f);
const glm::vec3 eye(2.0f, 2.0f, 3.0f);

// rotation globals
static GLfloat view_rotx = 20.f, view_roty = 30.f, view_rotz = 0.f;

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
   std::cout << key << std::endl;

   if (action != GLFW_PRESS) return;

   switch (key) {
   case GLFW_KEY_Z:
      if (mode & GLFW_MOD_SHIFT)
         view_rotz -= 5.0;
      else
         view_rotz += 5.0;
      break;
   case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(window, GLFW_TRUE);
      break;
   case GLFW_KEY_UP:
      view_rotx += 5.0;
      break;
   case GLFW_KEY_DOWN:
      view_rotx -= 5.0;
      break;
   case GLFW_KEY_LEFT:
      view_roty += 5.0;
      break;
   case GLFW_KEY_RIGHT:
      view_roty -= 5.0;
      break;
   default:
      return;
   };
}

// The MAIN function, from here we start the application and run the game loop
int main()
{
   std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
   // Init GLFW
   glfwInit();
   // Set all the required options for GLFW
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   // Create a GLFWwindow object that we can use for GLFW's functions
   GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Pacman by Christopher McArthur", nullptr, nullptr);
   if (window == nullptr)
   {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
   }
   glfwMakeContextCurrent(window);
   // Set the required callback functions
   glfwSetKeyCallback(window, key_callback);

   // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
   glewExperimental = GL_TRUE;
   // Initialize GLEW to setup the OpenGL Function pointers
   if (glewInit() != GLEW_OK)
   {
      std::cout << "Failed to initialize GLEW" << std::endl;
      return -1;
   }

   // Define the viewport dimensions
   int width, height;
   glfwGetFramebufferSize(window, &width, &height);

   glViewport(0, 0, width, height);

   projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.0f, 100.0f);

   // Build and compile our shader program
   // Vertex shader

   // Read the Vertex Shader code from the file
   string vertex_shader_path = "vertex.shader";
   string VertexShaderCode;
   std::ifstream VertexShaderStream(vertex_shader_path, ios::in);

   if (VertexShaderStream.is_open()) {
      string Line = "";
      while (getline(VertexShaderStream, Line))
         VertexShaderCode += "\n" + Line;
      VertexShaderStream.close();
   }
   else {
      printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
      getchar();
      exit(-1);
   }

   // Read the Fragment Shader code from the file
   string fragment_shader_path = "fragment.shader";
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

   std::vector<glm::vec3> vertices_cube;
   std::vector<glm::vec3> normals_cube;
   std::vector<glm::vec2> UVs_cube;
   loadOBJ("cube.obj", vertices_cube, normals_cube, UVs_cube); //read the vertices_cube from the cube.obj file

   GLuint VAO_cube;
   glGenVertexArrays(1, &VAO_cube);
   // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
   GLuint vertices_VBO, normals_VBO;
   glGenBuffers(1, &vertices_VBO);
   glGenBuffers(1, &normals_VBO);

   // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
   glBindVertexArray(VAO_cube);

   glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
   glBufferData(GL_ARRAY_BUFFER, vertices_cube.size() * sizeof(glm::vec3), &vertices_cube.front(), GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(0);

   glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
   glBufferData(GL_ARRAY_BUFFER, normals_cube.size() * sizeof(glm::vec3), &normals_cube.front(), GL_STATIC_DRAW);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(1);

   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glBindVertexArray(0); // Unbind VAO_cube (it's always a good thing to unbind any buffer/array to prevent strange bugs)

   // ----------------------------------------------------------------------------------------------------------------------------------------------
   // cmc-edit : This will be a test for drawing a the axises
   std::vector<glm::vec3> vertices_axis = { { -0.5f, 0.0f, 0.0f }, { 2.5f, 0.0f, 0.0f },    // cmc-edit : this is the start-end points for the x axis
                                            { 0.0f, -0.5f, 0.0f }, { 0.0f, 2.5f, 0.0f },    // cmc-edit : this is the start-end points for the y axis
                                            { 0.0f, 0.0f, -0.5f }, { 0.0f, 0.0f, 2.5f } };  // cmc-edit : this is the start-end points for the z axis

   std::vector<glm::vec3> colors_axis = { { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f },       // cmc-edit : red
                                          { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },       // cmc-edit : blue
                                          { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } };     // cmc-edit : green

   GLuint VAO_axis, VBO_axis, VBO_colors;    // cmc-edit : basic memory buffers
   glGenVertexArrays(1, &VAO_axis);          // cmc-edit : get mem_buf https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGenVertexArrays.xhtml should always be one for this usage
   glBindVertexArray(VAO_axis);              // cmc-edit : now we start to work with our mem_buf

   glGenBuffers(1, &VBO_axis);               // cmc-edit : associate buffer within index 0 (matches vertex.shader)
   glBindBuffer(GL_ARRAY_BUFFER, VBO_axis);  // cmc-edit : bind array buffer for use
   glBufferData(GL_ARRAY_BUFFER, vertices_axis.size() * sizeof(glm::vec3), &vertices_axis.front(), GL_STATIC_DRAW); // cmc-edit : load the vec of verticies
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); // cmc-edit : bind vertices at index 0
   glEnableVertexAttribArray(0);             // cmc-edit : close verticies at 0
   glBindBuffer(GL_ARRAY_BUFFER, 0);         // cmc-edit : close buffer

   glGenBuffers(1, &VBO_colors);             // cmc-edit : associate buffer within index 1 (matches vertex.shader)
   glBindBuffer(GL_ARRAY_BUFFER, VBO_colors);// cmc-edit : bind array buffer for use
   glBufferData(GL_ARRAY_BUFFER, colors_axis.size() * sizeof(glm::vec3), &colors_axis.front(), GL_STATIC_DRAW); // cmc-edit : load the vec of verticies
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); // cmc-edit : bind vertices at index 1
   glEnableVertexAttribArray(1);             // cmc-edit : close verticies at 1
   glBindBuffer(GL_ARRAY_BUFFER, 1);         // cmc-edit : close buffer

   glBindVertexArray(0);                     // cmc-edit : Unbind VAO_xaxis (it's always a good thing to unbind any buffer/array to prevent strange bugs)
   // ----------------------------------------------------------------------------------------------------------------------------------------------

   triangle_scale = glm::vec3(0.5f); // cmc-edit : this scales the view

   GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection_matrix");
   GLuint viewMatrixLoc = glGetUniformLocation(shaderProgram, "view_matrix");
   GLuint transformLoc = glGetUniformLocation(shaderProgram, "model_matrix");

   // Game loop
   while (!glfwWindowShouldClose(window))
   {
      // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
      glfwPollEvents();

      // Render
      // Clear the colorbuffer
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glm::mat4 view_matrix;
      view_matrix = glm::lookAt(eye, center, up);

      glm::mat4 model_matrix;
      model_matrix = glm::scale(model_matrix, triangle_scale);

      glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
      glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
      glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

      //glBindVertexArray(VAO_cube);
      //glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices_cube.size());
      //glBindVertexArray(0);

      // ----------------------------------------------------------------------------------------------------------------------------------------------
      glBindVertexArray(VAO_axis);                                   // cmc-edit : lets displays the axis
      glDrawArrays(GL_LINES, 0, (GLsizei)vertices_axis.size());      // cmc-edit : lets displays the axis
      glBindVertexArray(0);                                          // cmc-edit : lets displays the axis
      // ----------------------------------------------------------------------------------------------------------------------------------------------

      // Swap the screen buffers
      glfwSwapBuffers(window);
   }

   // Terminate GLFW, clearing any resources allocated by GLFW.
   glfwTerminate();
   return 0;
}
