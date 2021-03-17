#pragma once

#include <nana/gui/widgets/form.hpp>
#include "Config.h"
#include "subclass.h"
#include "Feedback.h"
#include "Requests.h"

#include "Hotkey.h"

class Globals {
public:
	nana::form fm;
	Config conf;
	Subclass sc;
	Feedback feedback;
	Requests req;
	Hotkey hotkey;

	Globals();
};