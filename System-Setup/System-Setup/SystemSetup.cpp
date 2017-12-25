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

#include "Script.h"
#include <iostream>
#include <string>

BOOL IsElevated();
std::wstring BrowseFolder(const std::wstring& saved_path);
static int BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
std::wstring GetPathToFile(const std::wstring & file_path);
BOOL TryToOpenFile(const std::wstring & full_path);

int main()
{
   std::cout << "Hello World!" << std::endl;

   if (!IsElevated())
   {
      std::cout << "Not running as admin, you need admin rights for this too work..." << std::endl << " Press 'enter' to exit" << std::endl;
      std::getline(std::cin, std::string());
      return -1;
   }

   std::cout << std::endl << "This setup application is to only configure your enviroment variables, you must manually install the libraries yourself." << std::endl
             << " If you have any question on what to do check out the repository this was developed in." << std::endl;

   Sleep(1000);

   SetEnvVarScript script;

   std::cout << std::endl << "First off, GLM 9.8.5 include directory..." << std::endl;
   script.AddVarAndPath(L"GLM_INC_DIR", GetPathToFile(L"\\glm\\glm.hpp"));

   std::cout << std::endl << "Now For GLEW 2.1.0 include directory..." << std::endl;
   script.AddVarAndPath(L"GLEW_INC_DIR", GetPathToFile(L"\\GL\\glew.h"));

   std::cout << std::endl << "Now For GLEW 2.1.0 library directory..." << std::endl;
   script.AddVarAndPath(L"GLEW_LIB_DIR", GetPathToFile(L"\\Debug\\glew32d.lib"));

   std::cout << std::endl << "Continuing with GLFW 3.2.1 include directory..." << std::endl;
   script.AddVarAndPath(L"GLFW_INC_DIR", GetPathToFile(L"\\GLFW\\glfw3.h"));

   std::cout << std::endl << "Continuing with GLFW 3.2.1 library directory..." << std::endl;
   script.AddVarAndPath(L"GLFW_LIB_DIR", GetPathToFile(L"\\Debug\\glfw3.lib"));

   std::cout << std::endl << "Moving on to FreeType 2.8.1 include directory..." << std::endl;
   script.AddVarAndPath(L"FT_INC_DIR", GetPathToFile(L"\\ft2build.h"));

   std::cout << std::endl << "Moving on to FreeType 2.8.1 library directory..." << std::endl;
   script.AddVarAndPath(L"FT_LIB_DIR", GetPathToFile(L"\\Debug\\freetype.lib"));

   std::cout << "Creating batch file...";
   if (!script.CreateScript())
   {
      std::cout << "   FAILED!" << std::endl << "Press 'enter' to exit." << std::endl;
      std::getline(std::cin, std::string());
      return -1;
   }
   std::cout << "   PASS!" << std::endl;

   if (!script.ExecuteScript())
   {
      std::cout << "FAILED to execute script!" << std::endl << "Press 'enter' to exit." << std::endl;
      std::getline(std::cin, std::string());
      return -1;
   }

   std::cout << "Successfully setup enviroment variables!" << std::endl << "  To use them you must logout and back in." << std::endl << "Press 'enter' to exit." << std::endl;
   std::getline(std::cin, std::string());

   return 0;
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
std::wstring BrowseFolder(const std::wstring& saved_path)
{
   TCHAR path[MAX_PATH];

   BROWSEINFO bi = { 0 };
   bi.lpszTitle = L"Browse for folder...";
   bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
   bi.lpfn = BrowseCallbackProc;
   bi.lParam = (LPARAM)saved_path.c_str();

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

      return std::wstring(path);
   }

   return L"";
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

std::wstring GetPathToFile(const std::wstring& file_path)
{
   static std::wstring last_dir_used = L"C:\\WINDOWS\\System32\\";
   std::wstring dir = BrowseFolder(last_dir_used);

   // https://stackoverflow.com/a/12097772/8480874
   std::cout << "Testing: " << std::string(dir.begin(), dir.end()) << " for " << std::string(file_path.begin(), file_path.end()) << "...";
   if (TryToOpenFile(dir + file_path))
   {
      std::cout << "  PASS!" << std::endl;
      last_dir_used = dir;
      return std::wstring(L"\"" + dir + L"\"\\"); // why extra backslash https://stackoverflow.com/questions/29190444/invalid-syntax-with-setx-for-having-more-than-two-arguments-when-there-are-onl && https://github.com/prince-chrismc/Computer-Graphics/issues/13
   }
   else
   {
      std::cout << " FAILED!" << std::endl << "   Please try again." << std::endl;
      return GetPathToFile(file_path);
   }
}

// https://msdn.microsoft.com/en-us/library/windows/desktop/bb540534(v=vs.85).aspx
BOOL TryToOpenFile(const std::wstring& full_path)
{
   const DWORD BUFFERSIZE = 5;
   HANDLE hFile;
   DWORD  dwBytesRead = 0;
   char   ReadBuffer[BUFFERSIZE] = { 0 };
   OVERLAPPED ol = { 0 };

   hFile = CreateFile(full_path.c_str(),        // file to open
      GENERIC_READ,                             // open for reading
      FILE_SHARE_READ,                          // share for reading
      NULL,                                     // default security
      OPEN_EXISTING,                            // existing file only
      FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // normal file
      NULL);                                    // no attr. template

   if (hFile == INVALID_HANDLE_VALUE)
   {
      return FALSE;
   }

   CloseHandle(hFile);
   return TRUE;
}
