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
#include <iostream>
#include <sstream>

// Function Declaration
const unsigned int GetUserInputOddNum(const unsigned int& lower, const unsigned int& upper);
const bool GetUserInputYesNo();

int main()
{
   std::cout << "Welcome to Pseudo Pac-Man! Simply chase food till your hearts content." << std::endl;

   do
   {
      std::cout << std::endl << "Please Select a grid size: (odd number) recommended: 21" << std::endl;
      unsigned int grid_size = GetUserInputOddNum(9, 35) - 1;

      Game::Engine game(grid_size);
      bool retval = game.Play();

      std::cout << (retval ? " You won! " : " Aww you lost. ") << "Would you like to play again? (y/n)" << std::endl;
   } while ( GetUserInputYesNo() );

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

const bool GetUserInputYesNo()
{
   do
   {
      std::cout << "Selcetion: ";
      std::string input;

      if (input == "y" || input == "Y" || input ==  "yes" || input == "Yes")
         return true;
      else if (input == "n" || input == "N" || input == "no" || input == "No")
         return false;
      else
         std::cout << "Invalid option. Please Try again..." << std::endl;

   } while (true);
}
