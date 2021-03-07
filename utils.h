#pragma once

#include <string>
#include <nana/gui/basis.hpp>

void CheckWinAPIErrorAndDisplay();
void TrayMessage(const nana::native_window_type& handle, std::string sTitle, std::string sInfo);
bool IsStringHex(const std::string& s);
std::string GetAppPath();