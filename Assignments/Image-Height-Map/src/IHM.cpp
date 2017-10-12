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

// enums
enum class ObjectColors { RED, GREEN, BLUE, GREY, YELLOW, TEAL };
enum class RenderMode { POINTS = GL_POINTS, LINES = GL_LINES, TRIANGLES = GL_TRIANGLES } g_RenderMode;

// Cursor
class Cursor final
{
   public:
      Cursor() {}

      static constexpr float Sensitivity = 0.05f;

      void SetLastX(const float& last) { m_Last.x = last; }
      void SetLastY(const float& last) { m_Last.y = last; }
      const float& GetLastX() { return m_Last.x; }
      const float& GetLastY() { return m_Last.y; }

      void ToggleLeftButton() { m_LeftActive ? m_LeftActive = false : m_LeftActive = true; }
      void ToggleMiddleButton() { m_MiddleActive ? m_MiddleActive = false : m_MiddleActive = true; }
      void ToggleRightButton() { m_RightActive ? m_RightActive = false : m_RightActive = true; }
      bool IsLeftActive() { return m_LeftActive; }
      bool IsMiddleActive() { return m_MiddleActive; }
      bool IsRightActive() { return m_RightActive; }

   private:
      glm::vec2 m_Last;
      bool m_LeftActive;
      bool m_MiddleActive;
      bool m_RightActive;

} g_Cursor;

// Camera
class Camera final
{
   public:
      Camera(const glm::vec3 center, const glm::vec3 up, const glm::vec3 eye) : m_center(center), m_up(up), m_eye(eye) { m_ViewMatrix = glm::lookAt(eye, center, up); }
      Camera() : Camera(glm::vec3(0.0f), glm::vec3(0.0,1.0,0.0), glm::vec3(-1.0,-1.0,-1.0)) {}

      void AdjustTranslateXAxis(const float& adj) { m_Trans.x += adj; RecalculateCameraPos(); }
      void AdjustTranslateYAxis(const float& adj) { m_Trans.y += adj; RecalculateCameraPos(); }
      void AdjustTranslateZAxis(const float& adj) { m_Trans.z += adj; RecalculateCameraPos(); }

      void AdjustRotateXAxis(const float& adj) { m_Rotate.x += adj; RecalculateCameraPos(); }
      void AdjustRotateYAxis(const float& adj) { m_Rotate.y += adj; RecalculateCameraPos(); }
      void AdjustRotateZAxis(const float& adj) { m_Rotate.z += adj; RecalculateCameraPos(); }

      void AdjustScalar(const float& adj)
      {
         m_Scalar += adj;

         if (m_Scalar < 0.5f)
            m_Scalar = 0.5f;

         if (m_Scalar > 5.0f)
            m_Scalar = 5.0f;

         RecalculateCameraPos();
      }

      void ResetCameraPos() { m_Trans = glm::vec3(0.0f); m_Rotate = glm::vec3(0.0f); m_Scalar = 0.0f; RecalculateCameraPos(); }

      glm::mat4 GetViewMatrix() { return m_ViewMatrix; }

   protected:
      void RecalculateCameraPos()
      {
         m_ViewMatrix = glm::lookAt(m_eye, m_center, m_up);
         m_ViewMatrix = glm::translate(m_ViewMatrix, m_Trans);
         m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
         m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
         m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
         m_ViewMatrix = glm::scale(m_ViewMatrix, glm::vec3(m_Scalar));
      }

   private:
      // home components
      glm::vec3 m_center;
      glm::vec3 m_up;
      glm::vec3 m_eye;

      // adjustment components
      glm::vec3 m_Trans;
      glm::vec3 m_Rotate;
      float m_Scalar;

      glm::mat4 m_ViewMatrix;
} g_Camera;

// Function Declaration
//const unsigned int GetUserInput(const unsigned int& lower, const unsigned int& upper);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);

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

      shaderProgram->SetShaderMat4("view_matrix", g_Camera.GetViewMatrix());

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
         g_Cursor.SetLastY(lastY);
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
         g_Cursor.SetLastY(lastY);
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
      g_Cursor.SetLastY(ypos);
      yoffset *= Cursor::Sensitivity;

      g_Camera.AdjustScalar(yoffset);
   }
   if (g_Cursor.IsRightActive())
   {
      float yoffset = static_cast<float>(g_Cursor.GetLastY() - ypos);
      g_Cursor.SetLastY(ypos);
      yoffset *= Cursor::Sensitivity;

      g_Camera.AdjustRotateYAxis(yoffset);
   }
}
