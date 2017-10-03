// SystemSetup.cpp : Defines the entry point for the console application.
//

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <sstream>
#include "SystemSetup.h"

int main()
{
   std::cout << "Hello World!" << std::endl;

   if (!IsElevated())
   {
      std::cout << "Not running as admin, you need admin rights for this too work..." << std::endl;
      return -1;
   }

   STARTUPINFO si;
   PROCESS_INFORMATION pi;

   ZeroMemory(&si, sizeof(si));
   si.cb = sizeof(si);
   ZeroMemory(&pi, sizeof(pi));
   if (!CreateProcess(NULL,
      L"cmd /C  hello.bat",
      NULL,
      NULL,
      FALSE,
      0,
      NULL,
      NULL,
      &si,
      &pi)
      )
   {
      printf("CreateProcess failed (%d)\n", GetLastError());
      return FALSE;
   }
   WaitForSingleObject(pi.hProcess, INFINITE);
   CloseHandle(pi.hProcess);
   CloseHandle(pi.hThread);

   return 0;
}

const uint16_t GetUserInput(const uint16_t & lower, const uint16_t & upper)
{
   // this is a gneral utility function to get a valid input between two values (inclusively)
   // adding this removed aprox. 200 lines of code lol
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

   } while (selection < lower || selection > upper);

   return selection;
}

BOOL IsElevated() 
{
   BOOL fRet = FALSE;
   HANDLE hToken = NULL;
   if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
      TOKEN_ELEVATION Elevation;
      DWORD cbSize = sizeof(TOKEN_ELEVATION);
      if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
         fRet = Elevation.TokenIsElevated;
      }
   }
   if (hToken) {
      CloseHandle(hToken);
   }
   return fRet;
}