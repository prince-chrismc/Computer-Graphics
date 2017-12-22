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
#include <vector>

class GameEngine
{
   public:
      GameEngine(const unsigned int& grid_size) : m_GridSize(grid_size), m_RenderMode(RenderMode::TRIANGLES){}
      ~GameEngine() = default;

   private:
      class InputTracker;
      class Game
      {
         public:
            Pacman pacman;
            std::vector<Food> Foods;
            std::vector<Alien> Aliens;
            XYZ_Axis axises;
            Grid grid(grid_size);
      };

      unsigned int m_GridSize;
      RenderMode m_RenderMode;

      Game m_Game;
};

class GameEngine::InputTracker
{
   public:
      InputTracker() = default;
      ~InputTracker() = default;

   private:
      bool zoom = false, tilt = false, pan = false;
      double lastX, lastY;
      const float sensitivity = 0.05f;
};
