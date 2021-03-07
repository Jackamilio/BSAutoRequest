#pragma once

#include "subclass.h"
#include "ClipboardBackup.h"
#include <nana/gui.hpp>
#include <nana/gui/timer.hpp>

class Hotkey {
private:
	void SendCopyInputs();

	subclass sc;
    HWND hwnd;
	bool itismyctrlc;
    nana::timer hotkeyalreadypressed;
    CClipboardBackup cbbk;
public:

	Hotkey(nana::form& fm);

	template<typename Function>
	bool Register(Function&& lambda) {
        // React to hotkey even when minimised
        if (RegisterHotKey(hwnd, 1, MOD_CONTROL | MOD_SHIFT | MOD_NOREPEAT, VK_F5)) {

            if (AddClipboardFormatListener(hwnd)) {
                sc.make_before(WM_HOTKEY, [=](UINT, WPARAM, LPARAM, LRESULT*) {
                    if (!hotkeyalreadypressed.started()) {
                        hotkeyalreadypressed.start();
                        cbbk.Backup();
                        SendCopyInputs();
                        itismyctrlc = true;
                    }
                    return true;
                    });
                // if the event triggers right in between at least this is fast
                sc.make_after(WM_CLIPBOARDUPDATE, [=](UINT, WPARAM, LPARAM, LRESULT*) {
                    if (itismyctrlc) {
                        itismyctrlc = false;
                        lambda();
                        cbbk.Restore();
                    }
                    return true;
                    });
                return true;
            }
        }
        return false;
	}
};