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
#include "RenderMode.h"
#include "Positioning.h"
#include <mutex>

class Alien final : public PositionMoveTowards
{
   public:
      Alien(float trans_x, float trans_y) { m_TransX = trans_x; m_TransY = trans_y; }
      ~Alien() = default;

      void Draw(const RenderMode& render_mode) const;
      void MoveTowards(float trans_x, float trans_y);

      static void IncrementScalar() { if (s_AlienScalar < BASE_SCALE_FACTOR*MAX_SCALE_COEFFICIENT) s_AlienScalar += BASE_SCALE_FACTOR; }
      static void DecrementScalar() { if (s_AlienScalar > BASE_SCALE_FACTOR*MIN_SCALE_COEFFICIENT) s_AlienScalar -= BASE_SCALE_FACTOR; }

   private:
      class Drawable;

      static float s_AlienScalar;
      static constexpr float BASE_SCALE_FACTOR = 0.125f;
      static constexpr float MAX_SCALE_COEFFICIENT = 10.0f;
      static constexpr float MIN_SCALE_COEFFICIENT = -1.0f;
      static constexpr float STEP_SIZE = 0.25f;
};

class Alien::Drawable
{
public:
   ~Drawable();
   Drawable(const Drawable&) = delete;
   Drawable& operator=(const Drawable&) = delete;

   static std::shared_ptr<Drawable> GetInstance() { std::call_once(s_Flag, []() { s_Instance.reset(new Drawable()); }); return s_Instance; }

   void Draw(const RenderMode& render_mode) const;

private:
   Drawable();

   GLuint m_VAO;
   GLuint m_Vertices;
   //GLuint m_Colors;

   GLsizei m_NumVertices;

   static std::once_flag s_Flag;
   static std::shared_ptr<Drawable> s_Instance;
};
