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

#pragma once

#include "Pacman.h"
#include "Food.h"
#include "Alien.h"
#include "Axis.h"
#include "Grid.h"
#include "GlfwWindow.h"
#include <vector>

namespace Game
{
   enum class Outcome
   {
      ALIEN_ATE_PACMAN,
      PACMAN_ATE_FOODS,
      PROGRESSION
   };

   class Engine;

   class Elements
   {
   friend class Engine;
   public:
      Elements(const unsigned int& grid_size);
      ~Elements() = default;

      void Draw(const RenderMode& render_mode) const;

      void IncrementScalars() const;
      void DecrementScalars() const;

      Outcome Process();

   private:
      Pacman m_Pacman;
      std::vector<Food> m_Foods;
      std::vector<Alien> m_Aliens;
      XYZ_Axis m_Axises;
      Grid m_Grid;
   };

   class Engine;

   // Sadly due to the limitations of GLFW callbacks this calls is just static
   class InputTracker abstract
   {
   public:
      static void SetEngine(Engine* engine) { s_Engine = engine; }

      static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
      static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
      static void cursor_callback(GLFWwindow* window, double xpos, double ypos);

   private:
      static Engine* s_Engine;

      static bool zoom, tilt, pan;
      static double lastX, lastY;
      static constexpr float SENSITIVITY = 0.05f;
   };

   class Initalizer
   {
      public:
         Initalizer();

         bool DidInitFail() { return m_Result == false; }

      protected:
         std::shared_ptr<GlfwWindow> m_Window;

      private:
         bool m_Result;

         static bool SetupShaders();
         static bool SetupGlew();
   };

   class Engine : private Initalizer
   {
   friend class InputTracker;
   public:
      Engine(const unsigned int& grid_size);
      ~Engine() { m_Window->CloseWindow(); };

      // Launches a new Pseudo-PacMan game
      // true if game won, otherwise false
      bool Play();


   private:
      unsigned int grid_size;
      RenderMode m_RenderMode;

      Elements m_Game;

      void MoveElements(const Pacman::Direction& dir);

      static void ClearFrame();
   };
}
