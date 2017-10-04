#pragma once

const uint16_t GetUserInput(const uint16_t & lower, const uint16_t & upper);
BOOL IsElevated();
std::string BrowseFolder(std::string saved_path);
static int BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

BOOL ExecuteScript();
