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

#include "Game.h"
#define SINGLETON_SHADER 1
#include "Shader.h"
#include "Shaders.h"
#include "Camera.h"

#include <random>
#include <iostream>
#include <string>

// ------------------------------------------------------------------------------------------------ //
//                                         Game::Elements                                           //
// ------------------------------------------------------------------------------------------------ //

Game::Elements::Elements(const unsigned int & grid_size) : m_Grid(grid_size)
{
   std::random_device rd;
   std::mt19937 rand_gen(rd());

   // Generate Foods
   unsigned int num_food = ((rand_gen() % 9) + 10)*(grid_size / 20);
   for (unsigned int new_food = 0; new_food <= num_food; new_food += 1)
   {
      m_Foods.emplace_back(float(float(rand_gen() % grid_size) - float(grid_size / 2))*0.25f, float(float(rand_gen() % grid_size) - float(grid_size / 2))*0.25f);
   }

   // Generate Aliens
   unsigned int num_alien = static_cast<unsigned int>(std::floor(4.0*(grid_size / 20.0)));
   for (unsigned int new_alien = 0; new_alien < num_alien; new_alien += 1)
   {
      m_Aliens.emplace_back(float(float(rand_gen() % grid_size) - float(grid_size / 2))*0.25f, float(float(rand_gen() % grid_size) - float(grid_size / 2))*0.25f);
   }
}

void Game::Elements::Draw(const RenderMode & render_mode) const
{
   m_Grid.Draw();
   m_Axises.Draw();
   for (Food food : m_Foods) food.Draw(render_mode);
   for (Alien alien : m_Aliens) alien.Draw(render_mode);
   m_Pacman.Draw(render_mode);
}

void Game::Elements::IncrementScalars() const
{
   Pacman::IncrementScalar();
   Alien::IncrementScalar();
   Food::IncrementScalar();
}

void Game::Elements::DecrementScalars() const
{
   Pacman::DecrementScalar();
   Alien::DecrementScalar();
   Food::DecrementScalar();
}

Game::Outcome Game::Elements::Process()
{
   for (std::vector<Food>::iterator itor = m_Foods.begin(); itor != m_Foods.end(); /* no itor */) // lets eat food =)
   {
      if (itor->ComparePosition(&m_Pacman))
         itor = m_Foods.erase(itor);
      else
         itor++;
   }

   for (auto alien = m_Aliens.begin(); alien != m_Aliens.end(); alien++) // lets not touch aliens =S
   {
      if (alien->ComparePosition(&m_Pacman))
      {
         return Outcome::ALIEN_ATE_PACMAN;
      }
   }

   if (m_Foods.empty()) // game won =)
   {
      return Outcome::PACMAN_ATE_FOODS;
   }

   return Outcome::PROGRESSION;
}

// ------------------------------------------------------------------------------------------------ //
//                                          Game::Engine                                            //
// ------------------------------------------------------------------------------------------------ //

Game::Outcome Game::Engine::Play()
{
   // Try to initialize the game engine
   if(DidInitFail() || !m_Window->IsValid()) return Outcome::SYSTEM_ERROR;

   auto shaderProgram = Shader::Linked::GetInstance();

   while (!m_Window->ShouldClose())
   {
      GlfwWindow::TriggerCallbacks();           // For all windows check callbacks
      ClearFrame();                             // Reset background color and z buffer test

      shaderProgram->SetUniformMat4("projection_matrix", m_Window->GetProjectionMatrix());
      shaderProgram->SetUniformMat4("view_matrix", Camera::GetInstance()->GetViewMatrix());

      // Main game logic
      m_Status = m_Game.Process();

      m_Game.Draw(m_RenderMode);
      m_Window->NextBuffer(); // Swap buffers

      switch (m_Status)
      {
      case Outcome::PROGRESSION: break;
      default: return m_Status;
      }

   }

   return Outcome::USER_EXIT;
}

void Game::Engine::MoveElements(const Pacman::Direction & dir)
{
   m_Game.m_Pacman.Move(dir, grid_size);

   for (auto alien = m_Game.m_Aliens.begin(); alien != m_Game.m_Aliens.end(); alien++)
   {
      alien->PositionMoveTowards::MoveTowards(&m_Game.m_Pacman);
   }
}

void Game::Engine::ClearFrame()
{
   glClearColor(0.05f, 0.075f, 0.075f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);             // Clear the depth buffer
}

// ------------------------------------------------------------------------------------------------ //
//                                       Game::InputTracker                                         //
// ------------------------------------------------------------------------------------------------ //

Game::Engine* Game::InputTracker::s_Engine = nullptr;
bool Game::InputTracker::zoom = false;
bool Game::InputTracker::tilt = false;
bool Game::InputTracker::pan = false;
double Game::InputTracker::lastX = 0.0;
double Game::InputTracker::lastY = 0.0;

void Game::InputTracker::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
   // we are only concerned about key presses not releases
   if (action != GLFW_PRESS)
      return;

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
      s_Engine->m_RenderMode = RenderMode::POINTS;
      break;
   case GLFW_KEY_L:
      s_Engine->m_RenderMode = RenderMode::LINES;
      break;
   case GLFW_KEY_T:
      s_Engine->m_RenderMode = RenderMode::TRIANGLES;
      break;

      // move m_Pacman
   case GLFW_KEY_D:
      s_Engine->MoveElements(Pacman::Direction::D_KEY);
      break;
   case GLFW_KEY_A:
      s_Engine->MoveElements(Pacman::Direction::A_KEY);
      break;
   case GLFW_KEY_S:
      s_Engine->MoveElements(Pacman::Direction::S_KEY);
      break;
   case GLFW_KEY_W:
      s_Engine->MoveElements(Pacman::Direction::W_KEY);
      break;
   default:
      return;
   };
}

void Game::InputTracker::mouse_callback(GLFWwindow * window, int button, int action, int mods)
{
   if (button == GLFW_MOUSE_BUTTON_LEFT) {
      if (action == GLFW_PRESS)
      {
         // zoom with mouse movement
         glfwGetCursorPos(window, &lastX, &lastY); // probly only on x (unspecified by a1)
         lastY = 0.0; // not required
         zoom = true;
      }
      else
      {
         lastX = 0.0;
         lastY = 0.0;
         zoom = false;
      }
   }
   else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
      if (action == GLFW_PRESS)
      {
         // tile-y with mouse movement
         glfwGetCursorPos(window, &lastX, &lastY);
         lastX = 0.0; // not required
         tilt = true;
      }
      else
      {
         lastX = 0.0;
         lastY = 0.0;
         tilt = false;
      }
   }
   else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
      if (action == GLFW_PRESS)
      {
         // pan on x movemnt (and y too)
         glfwGetCursorPos(window, &lastX, &lastY);
         lastY = 0.0; // not required
         pan = true;
      }
      else
      {
         lastX = 0.0;
         lastY = 0.0;
         pan = false;
      }
   }
}

void Game::InputTracker::cursor_callback(GLFWwindow * window, double xpos, double ypos)
{
   if (pan)
   {
      float xoffset = static_cast<float>(xpos - lastX);
      lastX = xpos;
      xoffset *= SENSITIVITY / 10.0f;

      Camera::GetInstance()->AdjustPanX(xoffset);
   }
   else if (tilt)
   {
      float yoffset = static_cast<float>(lastY - ypos);
      lastY = ypos;
      yoffset *= SENSITIVITY;

      Camera::GetInstance()->AdjustTiltY(yoffset);
   }
   else if (zoom)
   {
      float xoffset = static_cast<float>(xpos - lastX);
      lastX = xpos;
      xoffset *= SENSITIVITY;

      Camera::GetInstance()->AdjustZoomZ(xoffset);
   }
}

// ------------------------------------------------------------------------------------------------ //
//                                         Game::Initalizer                                         //
// ------------------------------------------------------------------------------------------------ //

std::shared_ptr<GlfwWindow> Game::Initalizer::m_Window = nullptr;

Game::Initalizer::Initalizer() : m_Result(true)
{
   static std::once_flag flag;
   std::call_once(flag, [this] {
      m_Window = GlfwWindowFactory::GetInstance()->CreateNewWindow("Pseudo Pac-Man - Munch away!");
      if (m_Window->IsValid())
      {
         m_Window->SetKeyCallback(InputTracker::key_callback);
         m_Window->SetMouseButtonCallback(InputTracker::mouse_callback);
         m_Window->SetCursorPosCallback(InputTracker::cursor_callback);
      }
      else m_Result = false;

      if (!SetupGlew()) m_Result = false;
      if (!SetupShaders()) m_Result = false;
   });
}

bool Game::Initalizer::SetupGlew()
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

bool Game::Initalizer::SetupShaders()
{
   Shader::Vertex vertexShader("shaders/vertex.shader");
   Shader::Fragment fragmentShader("shaders/fragment.shader");

   // make sure they are ready to use
   if (vertexShader() && fragmentShader())
      Shader::Linked::GetInstance()->Init(&vertexShader, &fragmentShader);

   return true;
}
