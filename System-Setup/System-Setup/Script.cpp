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

// http://www.cplusplus.com/forum/general/102587/#msg551994
// https://stackoverflow.com/a/10044348/8480874
BOOL Script::ExecuteScript()
{
   STARTUPINFO si;
   PROCESS_INFORMATION pi;
   LPTSTR szCmdline = _tcsdup(std::wstring(L"cmd /C " + m_FileName).c_str());

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

// https://msdn.microsoft.com/en-us/library/windows/desktop/bb540534(v=vs.85).aspx
// https://stackoverflow.com/a/21606502/8480874
BOOL SetEnvVarScript::CreateScript()
{
   HANDLE hFile;

   std::wstring script = L"@ECHO OFF\n\n:: Setting Env Vars For COMP371\n";

   for (auto env_path : m_VarsAndPaths)
   {
      script += std::wstring(L"setx " + env_path.first + L" " + env_path.second + L" /m\n");
   }

   std::string buff = std::string(script.begin(), script.end());

   DWORD dwBytesToWrite = (DWORD)strlen(buff.c_str());
   DWORD dwBytesWritten = 0;
   BOOL bErrorFlag = FALSE;

   hFile = CreateFile(m_FileName.c_str(),       // name of the write
      GENERIC_WRITE,                            // open for writing
      0,                                        // do not share
      NULL,                                     // default security
      CREATE_ALWAYS,                            // create new file only
      FILE_ATTRIBUTE_NORMAL,                    // normal file
      NULL);                                    // no attr. template

   if (hFile == INVALID_HANDLE_VALUE)
   {
      std::cout << "Terminal failure: Unable to create file \"" << GetPrintableFileName().c_str() << "\" for write.\n";
      return FALSE;
   }

   std::cout << "Writing " << dwBytesToWrite << " bytes to \"" << GetPrintableFileName().c_str() << "\"\n";

   bErrorFlag = WriteFile(hFile,                // open file handle
      buff.c_str(),                             // start of data to write
      dwBytesToWrite,                           // number of bytes to write
      &dwBytesWritten,                          // number of bytes that were written
      NULL);                                    // no overlapped structure

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
         std::cout << "Wrote " << dwBytesToWrite << " bytes to \"" << GetPrintableFileName().c_str() << "\" successfully.\n";
      }
   }

   CloseHandle(hFile);
   return TRUE;
}
