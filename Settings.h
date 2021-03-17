#pragma once

#include "gen_settings.h"
#include "Globals.h"
#include <nana/gui/timer.hpp>

class Settings : public Gen_settings {
private:
	Globals& g;

	nana::timer presskeytimer;
	int countdown;

	void SetHotkeyCaption();
public:
	Settings(Globals& globals);
};