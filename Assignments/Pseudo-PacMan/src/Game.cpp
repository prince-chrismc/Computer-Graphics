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


/*

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





























// Create a GLFWwindow
std::shared_ptr<GlfwWindow> window = GlfwWindowFactory::GetInstance()->CreateNewWindow("Pseudo Pac-Man - Munch away!");
if (!window->IsValid()) return ExitOnEnter();
SetCalbacks(window);

if (!SetupGlew()) return ExitOnEnter();

if (!SetupShaders()) return ExitOnEnter();
auto shaderProgram = ShaderLinker::GetInstance();

Line xaxis({ { -0.5f, 0.0f, 0.0f }, { 2.5f, 0.0f, 0.0f } }, ObjectColors::RED);
Line yaxis({ { 0.0f, -0.5f, 0.0f }, { 0.0f, 2.5f, 0.0f } }, ObjectColors::GREEN);
Line zaxis({ { 0.0f, 0.0f, -0.5f }, { 0.0f, 0.0f, 2.5f } }, ObjectColors::BLUE);
Grid m_Grid(grid_size);

// Elements loop
while (!window->ShouldClose())
{
GlfwWindow::TriggerCallbacks();           // For all windows check callbacks
ClearFrame();                             // Reset background color and z buffer test

shaderProgram->SetUniformMat4("projection_matrix", window->GetProjectionMatrix());
shaderProgram->SetUniformMat4("view_matrix", Camera::GetInstance()->GetViewMatrix());

m_Grid.Draw();
xaxis.Draw();
yaxis.Draw();
zaxis.Draw();

for (Food food : m_Foods) food.Draw(render_mode);
for (Alien alien : m_Aliens) alien.Draw(render_mode);
m_Pacman.Draw(render_mode);

// --------------------------------------------------------------------------------------------------------------------------------------------

/*                    *
*     GAME LOGIC     *
*                    *
for (std::vector<Food>::iterator itor = m_Foods.begin(); itor != m_Foods.end(); /* no itor *) // lets eat food =)
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
      ResetGame();
      break;
   }
}

if (m_Foods.empty()) // game won =)
{
   ResetGame();
}


window->NextBuffer(); // swap buffers
   }



























   void GenerateFood()
   {
   std::random_device rd;
   std::mt19937 rand_gen(rd());

   unsigned int num_food = ((rand_gen() % 9) + 10)*(grid_size / 20);
   for (unsigned int new_food = 0; new_food <= num_food; new_food += 1)
   {
   m_Foods.emplace_back(float(float(rand_gen() % grid_size) - float(grid_size / 2))*0.25f, float(float(rand_gen() % grid_size) - float(grid_size / 2))*0.25f);
   }
   }

   void GenerateAlien()
   {
   std::random_device rd;
   std::mt19937 rand_gen(rd());

   unsigned int num_alien = static_cast<unsigned int>(std::floor(4.0*(grid_size / 20.0)));
   for (unsigned int new_alien = 0; new_alien < num_alien; new_alien += 1)
   {
   m_Aliens.emplace_back(float(float(rand_gen() % grid_size) - float(grid_size / 2))*0.25f, float(float(rand_gen() % grid_size) - float(grid_size / 2))*0.25f);
   }
   }

   void ResetGame()
   {
   m_Foods.clear();
   m_Aliens.clear();

   GenerateFood();
   GenerateAlien();

   for (std::vector<Food>::iterator food = m_Foods.begin(); food != m_Foods.end(); )
   {
   for (Alien alien : m_Aliens)
   {
      if (food->ComparePosition(&alien))
      {
         food = m_Foods.erase(food);
      }
   }
   food++;
   }

   m_Pacman = Pacman();
}

void MoveAliens()
{
   for (auto alien = m_Aliens.begin(); alien != m_Aliens.end(); alien++)
   {
      alien->PositionMoveTowards::MoveTowards(&m_Pacman);
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
   glClear(GL_COLOR_BUFFER_BIT);             // Clear the depth buffer
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

      // move m_Pacman
   case GLFW_KEY_D:
      m_Pacman.Move(Pacman::Direction::D_KEY, grid_size);
      MoveAliens();
      break;
   case GLFW_KEY_A:
      m_Pacman.Move(Pacman::Direction::A_KEY, grid_size);
      MoveAliens();
      break;
   case GLFW_KEY_S:
      m_Pacman.Move(Pacman::Direction::S_KEY, grid_size);
      MoveAliens();
      break;
   case GLFW_KEY_W:
      m_Pacman.Move(Pacman::Direction::W_KEY, grid_size);
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
*/

#include "Shader.h"
#include "Camera.h"

#include <iostream>
#include <string>

// ------------------------------------------------------------------------------------------------ //
//                                         Game::Elements                                           //
// ------------------------------------------------------------------------------------------------ //

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

bool Game::Engine::Play()
{
   // Try to initialize the game engine
   if(!Init()) return false;

   auto shaderProgram = ShaderLinker::GetInstance();

   while (!m_Window->ShouldClose())
   {
      GlfwWindow::TriggerCallbacks();           // For all windows check callbacks
      ClearFrame();                             // Reset background color and z buffer test

      shaderProgram->SetUniformMat4("projection_matrix", m_Window->GetProjectionMatrix());
      shaderProgram->SetUniformMat4("view_matrix", Camera::GetInstance()->GetViewMatrix());

      m_Game.Draw(m_RenderMode);

      // Main game logic
      switch (m_Game.Process())
      {
      case Outcome::PACMAN_ATE_FOODS: return true;
      case Outcome::ALIEN_ATE_PACMAN: return false;
      default: break;
      }

      m_Window->NextBuffer(); // Swap buffers
   }

   return false;
}

bool Game::Engine::Init()
{
   // Create a GLFWwindow
   m_Window = GlfwWindowFactory::GetInstance()->CreateNewWindow("Pseudo Pac-Man - Munch away!");
   if (!m_Window->IsValid()) return ExitOnEnter();
   SetCalbacks();

   if (!SetupGlew()) return ExitOnEnter();

   if (!SetupShaders()) return ExitOnEnter();

   return true;
}

void Game::Engine::ClearFrame()
{
   glClearColor(0.05f, 0.075f, 0.075f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);             // Clear the depth buffer
}

bool Game::Engine::SetupGlew()
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

bool Game::Engine::SetupShaders()
{
   VertexShader vertexShader("shaders/vertex.shader");
   FragmentShader fragmentShader("shaders/fragment.shader");
   // make sure they are ready to use
   if (!vertexShader() || !fragmentShader()) return false;

   auto shaderProgram = ShaderLinker::GetInstance();
   if (!shaderProgram->Link(&vertexShader, &fragmentShader)) return false;

   return true;
}

bool Game::Engine::ExitOnEnter()
{
   std::cout << "Press 'enter' to exit." << std::endl;
   std::getline(std::cin, std::string());
   return false;
}

void Game::Engine::SetCalbacks()
{
   // Set the required callback functions
   m_Window->SetKeyCallback(key_callback);
   m_Window->SetMouseButtonCallback(mouse_callback);
   m_Window->SetCursorPosCallback(cursor_callback);
}
