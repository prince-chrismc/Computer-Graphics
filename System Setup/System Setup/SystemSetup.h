#pragma once

const uint16_t GetUserInput(const uint16_t & lower, const uint16_t & upper);
BOOL IsElevated();
std::string BrowseFolder(std::string saved_path);
static int BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
std::string GetPathToFile(const std::string & file_path);
BOOL TryToOpenFile(const std::string & full_path);
BOOL ExecuteScript();
