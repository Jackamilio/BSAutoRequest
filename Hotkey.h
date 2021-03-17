#pragma once

#include "ClipboardBackup.h"
#include <nana/gui/timer.hpp>

class Globals;

class Hotkey {
private:
	void SendCopyInputs();

    Globals& g;
    HWND hwnd;
	bool itismyctrlc;
    nana::timer hotkeyalreadypressed;
    CClipboardBackup cbbk;
public:

    Hotkey(Globals& globals);

	template<typename Function>
    void SetLambda(Function&& lambda);

    void RegisterFromConfig();
};
