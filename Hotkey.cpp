#include "Hotkey.h"

#include <Windows.h>

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

Hotkey::Hotkey(nana::form& fm, subclass& subc) :
    sc(subc),
    hwnd(reinterpret_cast<HWND>(fm.native_handle())),
    itismyctrlc(false),
    hotkeyalreadypressed(std::chrono::milliseconds{ 5 })
{
    hotkeyalreadypressed.elapse([=] {
            if (!(KeyIsDown(VK_CONTROL) && KeyIsDown(VK_C))) {
                hotkeyalreadypressed.stop();
            }
        });
}