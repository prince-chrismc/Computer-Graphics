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

class PositionMoveTowards;
class PositionMoveDirected;

class PositionComparable abstract
{
public:
   bool ComparePosition(float trans_x, float trans_y) const { return (m_TransX == trans_x) && (m_TransY == trans_y); }
   bool ComparePosition(PositionComparable* point) const { return (m_TransX == point->m_TransX) && (m_TransY == point->m_TransY); }

protected:
   float m_TransX;
   float m_TransY;

   friend class PositionMoveTowards;
   friend class PositionMoveDirected;
};

class PositionMoveable abstract : public PositionComparable
{
public:
   virtual void MoveTowards(PositionComparable* point) = 0;
};

class PositionMoveTowards abstract : public PositionMoveable
{
public:
   virtual void MoveTowards(float trans_x, float trans_y) = 0;
   void MoveTowards(PositionComparable* point) { MoveTowards(point->m_TransX, point->m_TransY); }
};

class PositionMoveDirected abstract : public PositionMoveable
{
public:
   enum class Direction { W_KEY = 270, D_KEY = 0, S_KEY = 90, A_KEY = 180 };
   virtual void Move(const Direction& dir, const unsigned int& grid_size) = 0;
   void MoveTowards(PositionComparable* point) { /* DO NOTHING */ }
};
