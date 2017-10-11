#pragma once

BOOL IsElevated();
std::string BrowseFolder(std::string saved_path);
static int BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
std::string GetPathToFile(const std::string & file_path);
BOOL TryToOpenFile(const std::string & full_path);
BOOL CreateScript(const std::string& glm_inc_dir, const std::string& glew_inc_dir, const std::string& glew_lib_dir, const std::string& glfw_inc_dir, const std::string& glfw_lib_dir);
BOOL ExecuteScript();
