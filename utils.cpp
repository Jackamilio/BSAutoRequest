#include "utils.h"

#include <Windows.h>
#include <strsafe.h>
#include <tchar.h>
#include <Shlwapi.h>

void CheckWinAPIErrorAndDisplay()
{
    DWORD dw = GetLastError();

    if (dw) {
        LPVOID lpMsgBuf;
        //LPVOID lpDisplayBuf;

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0, NULL);

        // Display the error message and exit the process

        //lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, lstrlen((LPCTSTR)lpMsgBuf) * sizeof(TCHAR));

        //StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("failed with error %d: %s"), dw, lpMsgBuf);
        MessageBox(NULL, (LPCTSTR)lpMsgBuf, TEXT("Error"), MB_OK);

        LocalFree(lpMsgBuf);
        //LocalFree(lpDisplayBuf);
    }
}


DWORD GetCbSize() {
    // Literal hell just to get the size of a structure.
    // I found this here https://app.box.com/s/vgdshsob0m6mcy0pftaj90wsxwer1uxu
    // from Castorix31 forum post here https://social.msdn.microsoft.com/Forums/en-US/2c23e14b-8998-4dd1-92f2-43a2b01c3dc4/notify-icon-balloon-tip?forum=winforms
    // I couldn't make the notification work without this.
    DLLVERSIONINFO verInfo = { 0 };
    typedef HRESULT(__stdcall* DllGetVersion) (DLLVERSIONINFO* pverInfo);
    HMODULE hShell32 = LoadLibrary(L"Shell32.dll");
    if (hShell32)
    {
        DllGetVersion pDllGetVersion = (DllGetVersion)GetProcAddress(hShell32, "DllGetVersion");
        verInfo.cbSize = sizeof(DLLVERSIONINFO);
        pDllGetVersion(&verInfo);
        FreeLibrary(hShell32);
    }
    if (verInfo.dwMajorVersion == 6 && verInfo.dwMinorVersion > 0)
        return sizeof(NOTIFYICONDATA);
    else if (verInfo.dwMajorVersion >= 6)
        return NOTIFYICONDATA_V3_SIZE;
    else if (verInfo.dwMajorVersion == 5)
        return NOTIFYICONDATA_V2_SIZE;
    else
        return NOTIFYICONDATA_V1_SIZE;
}

// Shows a notification. Windows only. Nana doesn't implement this.
// I still use nana::notifier to initialise the tray icon.
// Be aware a few things are hardcoded inside to coincide with nana's initialisation
void TrayMessage(const nana::native_window_type& handle, std::string sTitle, std::string sInfo)
{
    static const DWORD cbSize = GetCbSize();

    NOTIFYICONDATA nid = { 0 };
    nid.cbSize = cbSize;

    // Be aware of this : nana gave this uID to its notifier class because it's the first of an internal list
    // so I'm forced to hardcode it here. The scope of this program is really small so the list shouldn't grow
    // but it's a nice thing to be aware of as the program evolves.
    nid.uID = 0;
    // I found this value in nana source code in nana::detail::messages::tray, not accessible with includes
    nid.uCallbackMessage = 0x501;

    nid.hWnd = reinterpret_cast<HWND>(handle);

    nid.uFlags = NIF_MESSAGE | NIF_TIP | NIF_INFO;
    nid.dwInfoFlags = NIIF_USER | NIF_SHOWTIP;

    std::wcsncpy(nid.szInfoTitle, std::wstring(sTitle.begin(), sTitle.end()).c_str(), 64);
    std::wcsncpy(nid.szInfo, std::wstring(sInfo.begin(), sInfo.end()).c_str(), 256);

    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

bool IsStringHex(const std::string& s)
{
    for (auto it = s.begin(); it != s.end(); ++it)
    {
        const char ch = *it;
        if ((ch < '0' || ch > '9') &&
            (ch < 'a' || ch > 'f') &&
            (ch < 'A' || ch > 'F')) {
            return false;
        }
    }

    return true;
}

std::string GetAppPath() {
    // This path gets used to set the icon from the exe resource
    // This is black magic... I spent hours trying to find a way to do this through nana
    // while not using the windows api. Finally found something on the forums
    // http://nanapro.org/en-us/forum/index.php?u=/topic/297/ggset-icon-to-nana-gui-form/post-1162#post-1162
    // But still, this should be on the doc!! And it still uses GetModuleFileNameW which is windows api...
    std::wstring lapppath(4096, '\0');
    lapppath.resize(GetModuleFileName(0, &lapppath.front(), lapppath.size()));
    // downgrade wstring to string. This implementation should be fine, it's just a system path
    return std::string(lapppath.begin(), lapppath.end());
}