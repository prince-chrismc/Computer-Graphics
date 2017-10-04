// SystemSetup.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <sstream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shlobj.h>

#include "SystemSetup.h"

int main()
{
   std::cout << "Hello World!" << std::endl;

   if (!IsElevated())
   {
      std::cout << "Not running as admin, you need admin rights for this too work..." << std::endl << " Press 'enter' to exit" << std::endl;
      std::getline(std::cin, std::string());
      return -1;
   }

   std::cout << std::endl << "This setup application is to only configure your enviroment variables, you must manually install the libraries yourself." << std::endl << " If you have any question on what to do check out the repository this was developed in." << std::endl;

   Sleep(1000);

   std::cout << std::endl << "First of GLM 9.8.5 include directory..." << std::endl;
   std::string glm_inc_dir = BrowseFolder("C:\\WINDOWS\\System32\\");
   std::cout << "Testing: " << glm_inc_dir << "for glm\\glm.hpp...";
   if (TryToOpenFile(std::string(glm_inc_dir + "\\glm\\glm.hpp")))
   {
      std::cout << "  ... PASS!" << std::endl;
   }


   //if (ExecuteScript())
   //{
   //   std::cout << "Successfully setup enviroment variables!" << std::endl << "  To use them you must logout and back in." << std::endl;
   //}

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

// https://stackoverflow.com/a/8196291/8480874
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

// https://stackoverflow.com/a/21978398/8480874
// https://www.codeproject.com/Articles/2604/Browse-Folder-dialog-search-folder-and-all-sub-fol
std::string BrowseFolder(std::string saved_path)
{
   TCHAR path[MAX_PATH];

   const char* path_param = saved_path.c_str();

   BROWSEINFO bi = { 0 };
   bi.lpszTitle = L"Browse for folder...";
   bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
   bi.lpfn = BrowseCallbackProc;
   bi.lParam = (LPARAM)path_param;

   LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

   if (pidl != 0)
   {
      //get the name of the folder and put it in path
      SHGetPathFromIDList(pidl, path);

      //free memory used
      IMalloc * imalloc = 0;
      if (SUCCEEDED(SHGetMalloc(&imalloc)))
      {
         imalloc->Free(pidl);
         imalloc->Release();
      }

      // https://stackoverflow.com/a/12097772/8480874
      std::wstring raw(path);
      return std::string(raw.begin(), raw.end());
   }

   return "";
}

static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{

   if (uMsg == BFFM_INITIALIZED)
   {
      std::string tmp = (const char *)lpData;
      //std::cout << "path: " << tmp << std::endl;
      SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
   }

   return 0;
}

BOOL TryToOpenFile(const std::string& full_path)
{
   const DWORD BUFFERSIZE = 5;
   HANDLE hFile;
   DWORD  dwBytesRead = 0;
   char   ReadBuffer[BUFFERSIZE] = { 0 };
   OVERLAPPED ol = { 0 };

   std::wstring w_path(full_path.begin(), full_path.end());

   hFile = CreateFile(w_path.c_str(),               // file to open
      GENERIC_READ,          // open for reading
      FILE_SHARE_READ,       // share for reading
      NULL,                  // default security
      OPEN_EXISTING,         // existing file only
      FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // normal file
      NULL);                 // no attr. template

   if (hFile == INVALID_HANDLE_VALUE)
   {
      printf("Terminal failure: unable to open file \"%s\" for read.\n", full_path.c_str());
      return FALSE;
   }

   CloseHandle(hFile);
   return TRUE;
}

// http://www.cplusplus.com/forum/general/102587/#msg551994
BOOL ExecuteScript()
{
   STARTUPINFO si;
   PROCESS_INFORMATION pi;

   ZeroMemory(&si, sizeof(si));
   si.cb = sizeof(si);
   ZeroMemory(&pi, sizeof(pi));
   if (!CreateProcess(NULL, L"cmd /C  setup.bat",
      NULL, NULL, FALSE, 0, NULL, NULL,
      &si, &pi)
      )
   {
      printf("CreateProcess failed (%d)\n", GetLastError());
      return FALSE;
   }

   WaitForSingleObject(pi.hProcess, INFINITE);
   CloseHandle(pi.hProcess);
   CloseHandle(pi.hThread);

   return TRUE;
}
