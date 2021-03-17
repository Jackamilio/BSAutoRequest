#include "Hotkey.h"

#include <Windows.h>
#include "Globals.h"

#define VK_C 0x43

void Hotkey::SendCopyInputs() {
    INPUT inputs[6] = { 0 };

    int i = 0;
    auto addinput = [&inputs,&i] (WORD key, bool eventup) {
        inputs[i].type = INPUT_KEYBOARD;
        inputs[i].ki.wVk = key;
        inputs[i].ki.dwFlags = eventup ? KEYEVENTF_KEYUP : 0;
        ++i;
    };
    
    addinput(VK_SHIFT, true);       // shift up
    addinput(VK_MENU, true);        // alt up
    addinput(VK_CONTROL, false);    // ctrl down
    addinput(VK_C, false);          // c down
    addinput(VK_C, true);           // c up
    addinput(VK_CONTROL, true);     // ctrl up

    SendInput(i, inputs, sizeof(INPUT));

    //std::cout << "Input sent!!" << std::endl;
}

inline bool KeyIsDown(WORD key) {
    return GetAsyncKeyState(VK_CONTROL) & 0x01;
}

Hotkey::Hotkey(Globals& globals) : g(globals),
    hwnd(reinterpret_cast<HWND>(g.fm.native_handle())),
    itismyctrlc(false),
    hotkeyalreadypressed(std::chrono::milliseconds{ 5 })
{
    hotkeyalreadypressed.elapse([=] {
            if (!(KeyIsDown(VK_CONTROL) && KeyIsDown(VK_C))) {
                hotkeyalreadypressed.stop();
            }
        });
}

void Hotkey::RegisterFromConfig() {
    UnregisterHotKey(hwnd, 1);
    RemoveClipboardFormatListener(hwnd);

    if (g.conf.getBool(Config::BoolVars::hotkeyenabled)) {
        // React to hotkey even when minimised
        if (RegisterHotKey(hwnd, 1,
            (g.conf.getBool(Config::BoolVars::hotkeyctrl) ? MOD_CONTROL : 0) |
            (g.conf.getBool(Config::BoolVars::hotkeyshift) ? MOD_SHIFT : 0) |
            (g.conf.getBool(Config::BoolVars::hotkeyalt) ? MOD_ALT : 0) |
                MOD_NOREPEAT,
            g.conf.getNumAs<UINT>(Config::NumVars::hotkeycode))) {

            if (!AddClipboardFormatListener(hwnd)) {
                // cancel registration if this fails
                UnregisterHotKey(hwnd, 1);
            }
        }
    }
}