//example skeleton code
//modified from http://learnopengl.com/

#include "GL/glew.h"       // include GL Extension Wrangler
#include "GLFW/glfw3.h"    // include GLFW helper library
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <random>          //std::mt19937
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "objloader.hpp"   //include the object loader

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// OpenGL matrixes
glm::vec3 camera_position;
glm::vec3 model_scale;
glm::mat4 projection_matrix;

// Constant vectors
const glm::vec3 center(0.0f, 0.0f, 0.0f);
const glm::vec3 up(0.0f, 0.0f, 1.0f);
const glm::vec3 eye(1.0f, -5.0f, 3.0f);

// rotation, translation and scalar globals
float view_rotx = 0.0f, view_roty = 0.0f;                     // controll : arrow keys
float pacman_transx = 0.0f, pacman_transy = 0.0f;             // controll : wasd
float pacman_rotation_dec = 0.0f;                             // controll : wasd
const float pacman_viewing_offset_dec = 35.0f;
float pacman_scalar = 0.0f;                                  // controll : uj
float food_scalar = 0.0f;                                    // controll : uj

// enums
enum class ObjectColors { RED, GREEN, BLUE, GREY, YELLOW, TEAL };
enum class PacmanDirection { W_KEY = 180, D_KEY = 270, S_KEY = 0, A_KEY = 90 };

// dynamic user set values
GLuint grid_size = 20;

// mouse and cursor callback variables
bool zoom = false, tilt = false, pan = false;
double lastX, lastY;
float view_panx = 0.0f, view_tilty = 0.0f, view_zoomz = 1.0f;
const float sensitivity = 0.05f;

// food structure and variables
struct FoodPos
{
   float transx = 0.0f;
   float transy = 0.0f;

   FoodPos(float x, float y) { transx = x; transy = y; }
};
std::vector<FoodPos> Foods;
std::random_device rd;
std::mt19937 rand_gen(rd());

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

   float upper_move_limit = float((grid_size / 2.0)*0.25);
   float lower_move_limit = float(0 - ((grid_size / 2.0)*0.25));

   switch (key)
   {
   // windows close
   case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(window, GLFW_TRUE);
      break;

   // move camera
   case GLFW_KEY_UP:
      view_rotx += 1.0;
      break;
   case GLFW_KEY_DOWN:
      view_rotx -= 1.0;
      break;
   case GLFW_KEY_LEFT:
      view_roty += 1.0;
      break;
   case GLFW_KEY_RIGHT:
      view_roty -= 1.0;
      break;

   // reset everything
   case GLFW_KEY_HOME:
      view_rotx = 0.0;
      view_roty = 0.0;

      view_panx = 0.0;
      view_tilty = 0.0;
      view_zoomz = 1.0;
      break;

   // zoom in out
   case GLFW_KEY_U:
      pacman_scalar += 0.01f;
      food_scalar += 0.075f;
      break;
   case GLFW_KEY_J:
      if (pacman_scalar > -0.01f)
         pacman_scalar -= 0.01f;
      if(food_scalar > -0.075f)
         food_scalar -= 0.075f;
      break;

   // move pacman
   case GLFW_KEY_D:
      pacman_rotation_dec = (float)PacmanDirection::W_KEY + pacman_viewing_offset_dec;
      if (pacman_transx < upper_move_limit)
         pacman_transx += 0.25f;
      break;
   case GLFW_KEY_A:
      pacman_rotation_dec = (float)PacmanDirection::S_KEY + pacman_viewing_offset_dec;
      if (pacman_transx > lower_move_limit)
         pacman_transx -= 0.25f;
      break;
   case GLFW_KEY_S:
      pacman_rotation_dec = (float)PacmanDirection::D_KEY + pacman_viewing_offset_dec;
      if (pacman_transy > lower_move_limit)
         pacman_transy -= 0.25f;
      break;
   case GLFW_KEY_W:
      pacman_rotation_dec = (float)PacmanDirection::A_KEY + pacman_viewing_offset_dec;
      if (pacman_transy < upper_move_limit)
         pacman_transy += 0.25f;
      break;
   default:
      return;
   };
}

// inspiration https://stackoverflow.com/questions/37194845/using-glfw-to-capture-mouse-dragging-c for below
void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
   if (button == GLFW_MOUSE_BUTTON_LEFT) {
      if (action == GLFW_PRESS)
      {
         std::cout << "allow zooming" << std::endl;

         // zoom with mouse movement
         glfwGetCursorPos(window, &lastX, &lastY); // probly only on x (unspecified by a1)
         lastY = 0.0; // not required
         zoom = true;
      }
      else
      {
         std::cout << "disable zooming" << std::endl;
         lastX = 0.0;
         lastY = 0.0;
         zoom = false;
      }
   }
   else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
      if (action == GLFW_PRESS)
      {
         std::cout << "allow tilting" << std::endl;
         // tile-y with mouse movement
         glfwGetCursorPos(window, &lastX, &lastY);
         lastX = 0.0; // not required
         tilt = true;
      }
      else
      {
         std::cout << "disable tilting" << std::endl;
         lastX = 0.0;
         lastY = 0.0;
         tilt = false;
      }
   }
   else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
      if (action == GLFW_PRESS)
      {
         std::cout << "allow paning" << std::endl;
         // pan on x movemnt (and y too)
         glfwGetCursorPos(window, &lastX, &lastY);
         lastY = 0.0; // not required
         pan = true;
      }
      else
      {
         std::cout << "disable panning" << std::endl;
         lastX = 0.0;
         lastY = 0.0;
         pan = false;
      }
   }
}

// inspiration https://learnopengl.com/#!Getting-started/Camera
void cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
   if (pan)
   {
      float xoffset = static_cast<float>(xpos - lastX);
      lastX = xpos;
      xoffset *= sensitivity/10;
      
      view_panx += xoffset;

      std::cout << "panx: " << view_panx << std::endl;
   }
   else if (tilt)
   {
      float yoffset = static_cast<float>(lastY - ypos);
      lastY = ypos;
      yoffset *= sensitivity;

      view_tilty += yoffset;

      if (view_tilty > 89.0f)
         view_tilty = 89.0f;
      if (view_tilty < -89.0f)
         view_tilty = -89.0f;

      std::cout << "tilty: " << view_tilty << std::endl;
   }
   else if (zoom)
   {
      float xoffset = static_cast<float>(xpos - lastX);
      lastX = xpos;
      xoffset *= sensitivity;

      view_zoomz += xoffset;

      if(view_zoomz < 0.5f)
         view_zoomz = 0.5f;

      if (view_zoomz > 5.0f)
         view_zoomz = 5.0f;

      std::cout << "zoomz: " << view_zoomz << std::endl;
   }
}

void windows_callback(GLFWwindow* window, int width, int height)
{
   glViewport(0, 0, width, height);
   projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.0f, 100.0f);
}

// The MAIN function, from here we start the application and run the game loop
int main()
{
   // cmc-edit : get grid size

   // Init GLFW
   std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
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
   glfwSetMouseButtonCallback(window, mouse_callback);
   glfwSetCursorPosCallback(window, cursor_callback);
   glfwSetWindowSizeCallback(window, windows_callback);

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
   std::string vertex_shader_path = "vertex.shader";
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
   std::string fragment_shader_path = "fragment.shader";
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

   // -----------------------------------------------------------------------------------------------------------------------------------------------
   // cmc-edit : This will be a test for drawing a the xaxis
   std::vector<glm::vec3> vertices_xaxis = { { -0.5f, 0.0f, 0.0f }, { 2.5f, 0.0f, 0.0f } };  // cmc-edit : this is the start-end points for the x axis
   GLuint VAO_xaxis, VBO_xaxis;               // cmc-edit : basic memory buffers
   glGenVertexArrays(1, &VAO_xaxis);          // cmc-edit : get mem_buf https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGenVertexArrays.xhtml should always be one for this usage
   glBindVertexArray(VAO_xaxis);              // cmc-edit : now we start to work with our mem_buf

   glGenBuffers(1, &VBO_xaxis);               // cmc-edit : associate buffer within index 0 (matches vertex.shader)
   glBindBuffer(GL_ARRAY_BUFFER, VBO_xaxis);  // cmc-edit : bind array buffer for use
   glBufferData(GL_ARRAY_BUFFER, vertices_xaxis.size() * sizeof(glm::vec3), &vertices_xaxis.front(), GL_STATIC_DRAW); // cmc-edit : load the vec of verticies
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); // cmc-edit : bind vertices at index 0
   glEnableVertexAttribArray(0);             // cmc-edit : close verticies at 0
   glBindBuffer(GL_ARRAY_BUFFER, 0);         // cmc-edit : close buffer

   glBindVertexArray(0);                     // cmc-edit : Unbind VAO_xaxis (it's always a good thing to unbind any buffer/array to prevent strange bugs)

   // Y-axis ----------------------------------------------------------------------------------------------------------------------------------------
   std::vector<glm::vec3> vertices_yaxis = { { 0.0f, -0.5f, 0.0f },{ 0.0f, 2.5f, 0.0f } };
   GLuint VAO_yaxis, VBO_yaxis;
   glGenVertexArrays(1, &VAO_yaxis);
   glBindVertexArray(VAO_yaxis);

   glGenBuffers(1, &VBO_yaxis);
   glBindBuffer(GL_ARRAY_BUFFER, VBO_yaxis);
   glBufferData(GL_ARRAY_BUFFER, vertices_yaxis.size() * sizeof(glm::vec3), &vertices_yaxis.front(), GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glBindVertexArray(0);

   // Z-axis ----------------------------------------------------------------------------------------------------------------------------------------
   std::vector<glm::vec3> vertices_zaxis = { { 0.0f, 0.0f, -0.5f },{ 0.0f, 0.0f, 2.5f } };
   GLuint VAO_zaxis, VBO_zaxis;
   glGenVertexArrays(1, &VAO_zaxis);
   glBindVertexArray(VAO_zaxis);

   glGenBuffers(1, &VBO_zaxis);
   glBindBuffer(GL_ARRAY_BUFFER, VBO_zaxis);
   glBufferData(GL_ARRAY_BUFFER, vertices_zaxis.size() * sizeof(glm::vec3), &vertices_zaxis.front(), GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glBindVertexArray(0);
   // -----------------------------------------------------------------------------------------------------------------------------------------------
   // cmc-edit : this will be the grid lines
   std::vector<glm::vec3> vertices_grid;
   float half_grid(grid_size / 2.0f);
   float half_length(half_grid + 0.5f);

   for (unsigned int index = 0; index <= grid_size; index++)
   {
      // line of x-axis
      vertices_grid.emplace_back(glm::vec3(float(index - half_grid), 0 - half_length, 0.0f));
      vertices_grid.emplace_back(glm::vec3(float(index - half_grid), half_length, 0.0f));

      // line of y-axis
      vertices_grid.emplace_back(glm::vec3(0 - half_length, float(index - half_grid), 0.0f));
      vertices_grid.emplace_back(glm::vec3(half_length, float(index - half_grid), 0.0f));
   }

   GLuint VAO_grid, VBO_grid;
   glGenVertexArrays(1, &VAO_grid);
   glBindVertexArray(VAO_grid);

   glGenBuffers(1, &VBO_grid);
   glBindBuffer(GL_ARRAY_BUFFER, VBO_grid);
   glBufferData(GL_ARRAY_BUFFER, vertices_grid.size() * sizeof(glm::vec3), &vertices_grid.front(), GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glBindVertexArray(0);

   // cube (food) -----------------------------------------------------------------------------------------------------------------------------------
   std::vector<glm::vec3> cube_vertices;
   std::vector<glm::vec3> cube_normals;
   loadOBJ("cube.obj", cube_vertices, cube_normals, std::vector<glm::vec2>()); //read the cube_vertices from the cube.obj file

   GLuint VAO_cube;
   glGenVertexArrays(1, &VAO_cube);
   // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
   GLuint cube_vertices_VBO, cube_normals_VBO;
   glGenBuffers(1, &cube_vertices_VBO);
   glGenBuffers(1, &cube_normals_VBO);

   // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
   glBindVertexArray(VAO_cube);

   glBindBuffer(GL_ARRAY_BUFFER, cube_vertices_VBO);
   glBufferData(GL_ARRAY_BUFFER, cube_vertices.size() * sizeof(glm::vec3), &cube_vertices.front(), GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(0);

   glBindBuffer(GL_ARRAY_BUFFER, cube_normals_VBO);
   glBufferData(GL_ARRAY_BUFFER, cube_normals.size() * sizeof(glm::vec3), &cube_normals.front(), GL_STATIC_DRAW);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(1);

   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glBindVertexArray(0); // Unbind VAO_cube (it's always a good thing to unbind any buffer/array to prevent strange bugs)

   // pacman ----------------------------------------------------------------------------------------------------------------------------------------
   std::vector<glm::vec3> pacman_vertices;
   std::vector<glm::vec3> pacman_normals;
   loadOBJ("pacman.obj", pacman_vertices, pacman_normals, std::vector<glm::vec2>()); //read the pacman_vertices from the pacman.obj file 

   GLuint VAO_pacman;
   glGenVertexArrays(1, &VAO_pacman);
   // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s). 
   GLuint pacman_vertices_VBO, pacman_normals_VBO;
   glGenBuffers(1, &pacman_vertices_VBO);
   glGenBuffers(1, &pacman_normals_VBO);

   // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s). 
   glBindVertexArray(VAO_pacman);

   glBindBuffer(GL_ARRAY_BUFFER, pacman_vertices_VBO);
   glBufferData(GL_ARRAY_BUFFER, pacman_vertices.size() * sizeof(glm::vec3), &pacman_vertices.front(), GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(0);

   glBindBuffer(GL_ARRAY_BUFFER, pacman_normals_VBO);
   glBufferData(GL_ARRAY_BUFFER, pacman_normals.size() * sizeof(glm::vec3), &pacman_normals.front(), GL_STATIC_DRAW);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(1);

   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glBindVertexArray(0); // Unbind VAO_pacman (it's always a good thing to unbind any buffer/array to prevent strange bugs) 

   // -----------------------------------------------------------------------------------------------------------------------------------------------
   // -----------------------------------------------------------------------------------------------------------------------------------------------

   GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection_matrix");
   GLuint viewMatrixLoc = glGetUniformLocation(shaderProgram, "view_matrix");
   GLuint transformLoc = glGetUniformLocation(shaderProgram, "model_matrix");
   GLuint objectColorLoc = glGetUniformLocation(shaderProgram, "object_color");

   unsigned int num_food = ((rand_gen() % 9) + 10)*(grid_size/20);
   for (unsigned int new_food = 0; new_food <= num_food; new_food += 1)
   {
      Foods.emplace_back(FoodPos(float(float(rand_gen() % grid_size)-float(grid_size/2))*0.25f, float(float(rand_gen() % grid_size) - float(grid_size / 2))*0.25f));
   }

   // Game loop
   while (!glfwWindowShouldClose(window))
   {
      // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
      glfwPollEvents();

      // Render
      // Clear the colorbuffer
      glClearColor(0.05f, 0.075f, 0.075f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glm::mat4 view_matrix;
      view_matrix = glm::lookAt(eye, center, up);
      view_matrix = glm::translate(view_matrix, glm::vec3(view_panx, 0.0f, 0.0f));
      view_matrix = glm::rotate(view_matrix, glm::radians(view_tilty), glm::vec3(0.0f, 1.0f, 0.0f)); // apply tilt on y axis
      view_matrix = glm::scale(view_matrix, glm::vec3(view_zoomz));
      glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));

      glm::mat4 model_matrix;
      model_scale = glm::vec3(0.25f); // cmc-edit : this scales the objects
      model_matrix = glm::scale(model_matrix, model_scale);
      glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));

      // handled by callback
      glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

      // Cube -------------------------------------------------------------------------------------------------------------------------------------
      //glUniform1i(objectTypeLoc, 3);
      //glBindVertexArray(VAO_cube);
      //glDrawArrays(GL_TRIANGLES, 0, (GLsizei)cube_vertices.size());
      //glBindVertexArray(0);
      // Grid -------------------------------------------------------------------------------------------------------------------------------------
      glUniform1i(objectColorLoc, (GLint)ObjectColors::GREY);
      glBindVertexArray(VAO_grid);
      glDrawArrays(GL_LINES, 0, (GLsizei)vertices_grid.size());
      glBindVertexArray(0);
      // X-axis -------------------------------------------------------------------------------------------------------------------------------------
      glUniform1i(objectColorLoc, (GLint)ObjectColors::RED);
      glBindVertexArray(VAO_xaxis);                                   // cmc-edit : lets displays the axis
      glDrawArrays(GL_LINES, 0, (GLsizei)vertices_xaxis.size());      // cmc-edit : lets displays the axis
      glBindVertexArray(0);                                           // cmc-edit : lets displays the axis
      // Y-axis -------------------------------------------------------------------------------------------------------------------------------------
      glUniform1i(objectColorLoc, (GLint)ObjectColors::GREEN);
      glBindVertexArray(VAO_yaxis);                                   // cmc-edit : lets displays the axis
      glDrawArrays(GL_LINES, 0, (GLsizei)vertices_yaxis.size());      // cmc-edit : lets displays the axis
      glBindVertexArray(0);                                           // cmc-edit : lets displays the axis
      // Z-axis -------------------------------------------------------------------------------------------------------------------------------------
      glUniform1i(objectColorLoc, (GLint)ObjectColors::BLUE);
      glBindVertexArray(VAO_zaxis);                                   // cmc-edit : lets displays the axis
      glDrawArrays(GL_LINES, 0, (GLsizei)vertices_zaxis.size());      // cmc-edit : lets displays the axis
      glBindVertexArray(0);                                           // cmc-edit : lets displays the axis

      // foods --------------------------------------------------------------------------------------------------------------------------------------
      for each (FoodPos food in Foods)
      {
         glm::mat4 food_model_matrix;
         glm::vec3 food_scale(0.075f + food_scalar); // cmc-edit : this scales the object
         food_model_matrix = glm::translate(food_model_matrix, glm::vec3(food.transx, food.transy, 0.0f));
         food_model_matrix = glm::scale(food_model_matrix, food_scale);
         glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(food_model_matrix));

         glUniform1i(objectColorLoc, (GLint)ObjectColors::YELLOW);
         glBindVertexArray(VAO_cube);
         glDrawArrays(GL_TRIANGLES, 0, (GLsizei)cube_vertices.size());
         glBindVertexArray(0);
      }

      // pacman -------------------------------------------------------------------------------------------------------------------------------------
      glm::mat4 pacman_model_matrix;
      glm::vec3 pacman_scale(0.01f + pacman_scalar); // cmc-edit : this scales the object
      pacman_model_matrix = glm::translate(pacman_model_matrix, glm::vec3(pacman_transx, pacman_transy, 0.0f));                   // cmc-edit : inspiration https://learnopengl.com/#!Getting-started/Transformations
      pacman_model_matrix = glm::rotate(pacman_model_matrix, glm::radians(pacman_rotation_dec), glm::vec3(0.0f, 0.0f, 1.0f));     // cmc-edit : inspiration https://learnopengl.com/#!Getting-started/Transformations
      pacman_model_matrix = glm::scale(pacman_model_matrix, pacman_scale);                                                        // cmc-edit : inspiration https://learnopengl.com/#!Getting-started/Transformations
      glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(pacman_model_matrix));

      glUniform1i(objectColorLoc, (GLint)ObjectColors::YELLOW);
      glBindVertexArray(VAO_pacman);
      glDrawArrays(GL_TRIANGLES, 0, (GLsizei)pacman_vertices.size());
      glBindVertexArray(0);
      // --------------------------------------------------------------------------------------------------------------------------------------------

      // Swap the screen buffers
      glfwSwapBuffers(window);
   }

   // Terminate GLFW, clearing any resources allocated by GLFW.
   glfwTerminate();
   return 0;
}
