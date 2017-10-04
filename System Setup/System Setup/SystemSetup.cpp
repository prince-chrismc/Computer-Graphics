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
      std::cout << "Not running as admin, you need admin rights for this too work..." << std::endl;
      return -1;
   }

   std::cout << std::endl << "This setup application is to only configure your enviroment variables, you must manually install the libraries yourself." << std::endl;


   std::cout << BrowseFolder("C:\\WINDOWS\\System32\\") << std::endl;



   if (ExecuteScript())
   {
      std::cout << "Successfully setup enviroment variables!" << std::endl << "  To use them you must logout and back in." << std::endl;
   }

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
      std::cout << "path: " << tmp << std::endl;
      SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
   }

   return 0;
}

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
