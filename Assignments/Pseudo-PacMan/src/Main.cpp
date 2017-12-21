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
#include <sstream>
#include <vector>
#include <random>                               //std::mt19937

#include "GL/glew.h"                            // include GL Extension Wrangler
#include "glm/gtc/matrix_transform.hpp"         //glm::lookAt

#include "objloader.h"
#include "GlfwWindow.h"
#include "Shader.h"
#include "RenderMode.h"
#include "ObjectColors.h"
#include "Grid.h"
#include "Food.h"
#include "Axis.h"
#include "Alien.h"
#include "Pacman.h"
#include "Camera.h"

// dynamic user set values
GLuint grid_size = 20;
RenderMode render_mode(RenderMode::TRIANGLES);

// mouse and cursor callback variables
bool zoom = false, tilt = false, pan = false;
double lastX, lastY;
const float sensitivity = 0.05f;

// Game Elements
Pacman pacman;
std::vector<Food> Foods;
std::vector<Alien> Aliens;

// Function Declaration
const unsigned int GetUserInputOddNum(const unsigned int& lower, const unsigned int& upper);
void GenerateFood();
void GenerateAlien();
void ResetGame();
void MoveAliens();
void ClearFrame();
bool SetupGlew();
bool SetupShaders();
int ExitOnEnter();
void SetCalbacks(std::shared_ptr<GlfwWindow> window);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);

int main()
{
   std::cout << "Welcome to Pseudo Pac-Man! Simply chase food till your hearts content." << std::endl;

   std::cout << std::endl << "Please Select a grid size: (odd number) recommended: 21" << std::endl;
   unsigned int grid_size = GetUserInputOddNum(9, 35) - 1;

   // Create a GLFWwindow
   std::shared_ptr<GlfwWindow> window = GlfwWindowFactory::GetInstance()->CreateNewWindow("Pseudo Pac-Man - Munch away!");
   if (!window->IsValid()) return ExitOnEnter();
   SetCalbacks(window);

   if (!SetupGlew()) return ExitOnEnter();

   if (!SetupShaders()) return ExitOnEnter();
   auto shaderProgram = ShaderLinker::GetInstance();

   Axis xaxis({ { -0.5f, 0.0f, 0.0f }, { 2.5f, 0.0f, 0.0f } }, ObjectColors::RED);
   Axis yaxis({ { 0.0f, -0.5f, 0.0f }, { 0.0f, 2.5f, 0.0f } }, ObjectColors::GREEN);
   Axis zaxis({ { 0.0f, 0.0f, -0.5f }, { 0.0f, 0.0f, 2.5f } }, ObjectColors::BLUE);
   Grid grid(grid_size);

   // Game loop
   while (!window->ShouldClose())
   {
      GlfwWindow::TriggerCallbacks();           // For all windows check callbacks
      ClearFrame();                             // Reset background color and z buffer test

      shaderProgram->SetUniformMat4("projection_matrix", window->GetProjectionMatrix());
      shaderProgram->SetUniformMat4("view_matrix", Camera::GetInstance()->GetViewMatrix());

      grid.Draw();
      xaxis.Draw();
      yaxis.Draw();
      zaxis.Draw();

      for (Food food : Foods) food.Draw(render_mode);
      for (Alien alien : Aliens) alien.Draw(render_mode);
      pacman.Draw(render_mode);

      // --------------------------------------------------------------------------------------------------------------------------------------------

      /*                    *
       *     GAME LOGIC     *
       *                    */
      for (std::vector<Food>::iterator itor = Foods.begin(); itor != Foods.end(); /* no itor */) // lets eat food =)
      {
         if (itor->ComparePosition(&pacman))
            itor = Foods.erase(itor);
         else
            itor++;
      }

      for (auto alien = Aliens.begin(); alien != Aliens.end(); alien++) // lets not touch aliens =S
      {
         if (alien->ComparePosition(&pacman))
         {
            ResetGame();
            break;
         }
      }

      if (Foods.empty()) // game won =)
      {
         ResetGame();
      }


      window->NextBuffer(); // swap buffers
   }

   return 0;
}

// ------------------------------------------------------------------------------------------------ //
//                                          UTILITY CODE                                          - //
// ------------------------------------------------------------------------------------------------ //

/// copied from another project https://github.com/prince-chrismc/Pandemic/blob/master/Pandemic/Sources/GameEngine.cpp small modifications
const unsigned int GetUserInputOddNum(const unsigned int& lower, const unsigned int& upper)
{
   uint16_t selection = 0;
   do
   {
      std::cout << "Selcetion: ";
      std::string input;
      std::getline(std::cin, input);
      std::stringstream ss(input);
      ss >> selection;

      if (selection < lower || selection > upper)
         std::cout << "Invalid option. Please Try again..." << std::endl;

   } while (selection < lower || selection > upper || selection % 2 != 1);

   return selection;
}

void GenerateFood()
{
   std::random_device rd;
   std::mt19937 rand_gen(rd());

   unsigned int num_food = ((rand_gen() % 9) + 10)*(grid_size / 20);
   for (unsigned int new_food = 0; new_food <= num_food; new_food += 1)
   {
      Foods.emplace_back(float(float(rand_gen() % grid_size) - float(grid_size / 2))*0.25f, float(float(rand_gen() % grid_size) - float(grid_size / 2))*0.25f);
   }
}

void GenerateAlien()
{
   std::random_device rd;
   std::mt19937 rand_gen(rd());

   unsigned int num_alien = static_cast<unsigned int>(std::floor(4.0*(grid_size / 20.0)));
   for (unsigned int new_alien = 0; new_alien < num_alien; new_alien += 1)
   {
      Aliens.emplace_back(float(float(rand_gen() % grid_size) - float(grid_size / 2))*0.25f, float(float(rand_gen() % grid_size) - float(grid_size / 2))*0.25f);
   }
}

void ResetGame()
{
   Foods.clear();
   Aliens.clear();

   GenerateFood();
   GenerateAlien();

   for (std::vector<Food>::iterator food = Foods.begin(); food != Foods.end(); /* no itor */)
   {
      for (Alien alien : Aliens)
      {
         if (food->ComparePosition(&alien))
         {
            food = Foods.erase(food);
         }
      }
      food++;
   }

   pacman = Pacman();
}

void MoveAliens()
{
   for (auto alien = Aliens.begin(); alien != Aliens.end(); alien++)
   {
      alien->PositionMoveTowards::MoveTowards(&pacman);
   }
}

bool SetupShaders()
{
   VertexShader vertexShader("shaders/vertex.shader");
   FragmentShader fragmentShader("shaders/fragment.shader");
   // make sure they are ready to use
   if (!vertexShader() || !fragmentShader()) return false;

   auto shaderProgram = ShaderLinker::GetInstance();
   if (!shaderProgram->Link(&vertexShader, &fragmentShader)) return false;

   return true;
}

void SetCalbacks(std::shared_ptr<GlfwWindow> window)
{
   // Set the required callback functions
   window->SetKeyCallback(key_callback);
   window->SetMouseButtonCallback(mouse_callback);
   window->SetCursorPosCallback(cursor_callback);
}

int ExitOnEnter()
{
   std::cout << "Press 'enter' to exit." << std::endl;
   std::getline(std::cin, std::string());
   return -1;
}

void ClearFrame()
{
   glClearColor(0.05f, 0.075f, 0.075f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/);             // Clear the depth buffer
}

bool SetupGlew()
{
   // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
   glewExperimental = GL_TRUE;
   // Initialize GLEW to setup the OpenGL Function pointers
   if (glewInit() != GLEW_OK)
   {
      std::cout << "Failed to initialize GLEW" << std::endl;
      return false;
   }
   return true;
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
      Camera::GetInstance()->IncrementRotationX();
      break;
   case GLFW_KEY_DOWN:
      Camera::GetInstance()->DecrementRotationX();
      break;
   case GLFW_KEY_LEFT:
      Camera::GetInstance()->IncrementRotationY();
      break;
   case GLFW_KEY_RIGHT:
      Camera::GetInstance()->DecrementRotationY();
      break;

      // reset everything
   case GLFW_KEY_HOME:
      Camera::GetInstance()->Reset();
      break;

      // scaling
   case GLFW_KEY_U:
      Pacman::IncrementScalar();
      Alien::IncrementScalar();
      Food::IncrementScalar();
      break;
   case GLFW_KEY_J:
      Pacman::DecrementScalar();
      Alien::DecrementScalar();
      Food::DecrementScalar();
      break;

      // render mode
   case GLFW_KEY_P:
      render_mode = RenderMode::POINTS;
      break;
   case GLFW_KEY_L:
      render_mode = RenderMode::LINES;
      break;
   case GLFW_KEY_T:
      render_mode = RenderMode::TRIANGLES;
      break;

      // move pacman
   case GLFW_KEY_D:
      pacman.Move(Pacman::Direction::D_KEY, grid_size);
      MoveAliens();
      break;
   case GLFW_KEY_A:
      pacman.Move(Pacman::Direction::A_KEY, grid_size);
      MoveAliens();
      break;
   case GLFW_KEY_S:
      pacman.Move(Pacman::Direction::S_KEY, grid_size);
      MoveAliens();
      break;
   case GLFW_KEY_W:
      pacman.Move(Pacman::Direction::W_KEY, grid_size);
      MoveAliens();
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
      xoffset *= sensitivity / 10.0f;

      Camera::GetInstance()->AdjustPanX(xoffset);
   }
   else if (tilt)
   {
      float yoffset = static_cast<float>(lastY - ypos);
      lastY = ypos;
      yoffset *= sensitivity;

      Camera::GetInstance()->AdjustTiltY(yoffset);
   }
   else if (zoom)
   {
      float xoffset = static_cast<float>(xpos - lastX);
      lastX = xpos;
      xoffset *= sensitivity;

      Camera::GetInstance()->AdjustZoomZ(xoffset);
   }
}
