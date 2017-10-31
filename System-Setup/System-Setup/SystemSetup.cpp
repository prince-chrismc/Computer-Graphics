// SystemSetup.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <sstream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shlobj.h>
#include <tchar.h>

#include "SystemSetup.h"                        // only contains func declarations

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
   std::string glm_inc_dir = GetPathToFile("\\glm\\glm.hpp");

   std::cout << std::endl << "Now For GLEW 2.1.0 include directory..." << std::endl;
   std::string glew_inc_dir = GetPathToFile("\\GL\\glew.h");

   std::cout << std::endl << "Now For GLEW 2.1.0 library directory..." << std::endl;
   std::string glew_lib_dir = GetPathToFile("\\Debug\\glew32d.lib");

   std::cout << std::endl << "Continuing with GLFW 3.2.1 include directory..." << std::endl;
   std::string glfw_inc_dir = GetPathToFile("\\GLFW\\glfw3.h");

   std::cout << std::endl << "Continuing with GLFW 3.2.1 library directory..." << std::endl;
   std::string glfw_lib_dir = GetPathToFile("\\Debug\\glfw3.lib");

   std::cout << "Creating batch file...";
   if (!CreateScript(glm_inc_dir, glew_inc_dir, glew_lib_dir, glfw_inc_dir, glfw_lib_dir))
   {
      std::cout << "   FAILED!" << std::endl;
      return -1;
   }
   std::cout << "   PASS!" << std::endl;

   if (!ExecuteScript())
   {
      std::cout << "FAILED to execute script!" << std::endl;
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

std::string GetPathToFile(const std::string& file_path)
{

   std::string dir = BrowseFolder("C:\\WINDOWS\\System32\\");
   std::cout << "Testing: " << dir << " for " << file_path << "...";
   if (TryToOpenFile(dir + file_path))
   {
      std::cout << "  PASS!" << std::endl;
      return "\"" + dir += "\"\\"; // why extra backslash https://stackoverflow.com/questions/29190444/invalid-syntax-with-setx-for-having-more-than-two-arguments-when-there-are-onl && https://github.com/prince-chrismc/Computer-Graphics/issues/13
   }
   else
   {
      std::cout << " FAILED!" << std::endl << "   Please try again." << std::endl;
      return GetPathToFile(file_path);
   }
}

// https://msdn.microsoft.com/en-us/library/windows/desktop/bb540534(v=vs.85).aspx
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
      //printf("Terminal failure: unable to open file \"%s\" for read.\n", full_path.c_str());
      return FALSE;
   }

   CloseHandle(hFile);
   return TRUE;
}

// https://msdn.microsoft.com/en-us/library/windows/desktop/bb540534(v=vs.85).aspx
// https://stackoverflow.com/a/21606502/8480874
BOOL CreateScript(const std::string& glm_inc_dir, const std::string& glew_inc_dir, const std::string& glew_lib_dir, const std::string& glfw_inc_dir, const std::string& glfw_lib_dir)
{
   HANDLE hFile;

   std::string script = "@ECHO OFF\n\n:: Setting Env Vars For COMP371\nsetx GLM_INC_DIR " + glm_inc_dir + " /m\nsetx GLEW_INC_DIR " + glew_inc_dir + " /m\nsetx GLFW_INC_DIR " + glfw_inc_dir + " /m\nsetx GLFW_LIB_DIR " + glfw_lib_dir + " /m\nsetx GLEW_LIB_DIR " + glew_lib_dir + " /m\n";

   DWORD dwBytesToWrite = (DWORD)strlen(script.c_str());
   DWORD dwBytesWritten = 0;
   BOOL bErrorFlag = FALSE;

   hFile = CreateFile(L"setup.bat",           // name of the write
                      GENERIC_WRITE,          // open for writing
                      0,                      // do not share
                      NULL,                   // default security
                      CREATE_ALWAYS,          // create new file only
                      FILE_ATTRIBUTE_NORMAL,  // normal file
                      NULL);                  // no attr. template

   if (hFile == INVALID_HANDLE_VALUE)
   {
      printf("Terminal failure: Unable to create file \"setup.bat\" for write.\n");
      return FALSE;
   }

   printf("Writing %d bytes to %s.\n", dwBytesToWrite, "setup.bat");

   bErrorFlag = WriteFile(hFile,            // open file handle
                          script.c_str(),   // start of data to write
                          dwBytesToWrite,   // number of bytes to write
                          &dwBytesWritten,  // number of bytes that were written
                          NULL);            // no overlapped structure

   if (FALSE == bErrorFlag)
   {
      printf("Terminal failure: Unable to write to file.\n");
      return FALSE;
   }
   else
   {
      if (dwBytesWritten != dwBytesToWrite)
      {
         // This is an error because a synchronous write that results in
         // success (WriteFile returns TRUE) should write all data as
         // requested. This would not necessarily be the case for
         // asynchronous writes.
         printf("Error: dwBytesWritten != dwBytesToWrite\n");
         return FALSE;
      }
      else
      {
         printf("Wrote %d bytes to %s successfully.\n", dwBytesWritten, "setup.bat");
      }
   }

   CloseHandle(hFile);
   return TRUE;
}

// http://www.cplusplus.com/forum/general/102587/#msg551994
// https://stackoverflow.com/a/10044348/8480874
BOOL ExecuteScript()
{
   STARTUPINFO si;
   PROCESS_INFORMATION pi;
   LPTSTR szCmdline = _tcsdup(TEXT("cmd /C  setup.bat"));

   ZeroMemory(&si, sizeof(si));
   si.cb = sizeof(si);
   ZeroMemory(&pi, sizeof(pi));
   if (!CreateProcess(NULL, szCmdline,
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
