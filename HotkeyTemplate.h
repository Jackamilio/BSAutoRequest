#pragma once

#include "Hotkey.h"
#include "Globals.h"

template<typename Function>
inline void Hotkey::SetLambda(Function&& lambda) {
    g.sc.make_before(WM_HOTKEY, [=](UINT, WPARAM, LPARAM, LRESULT*) {
        if (!hotkeyalreadypressed.started()) {
            hotkeyalreadypressed.start();
            cbbk.Backup();
            SendCopyInputs();
            itismyctrlc = true;
        }
        return true;
        });
    // if the event triggers right in between at least this is fast
    g.sc.make_after(WM_CLIPBOARDUPDATE, [=](UINT, WPARAM, LPARAM, LRESULT*) {
        if (itismyctrlc) {
            itismyctrlc = false;
            lambda();
            cbbk.Restore();
        }
        return true;
        });
}