#include "Globals.h"

Globals::Globals() :
	fm{ nana::API::make_center(300, 200), nana::appearance(true,true,false,false,true,false,false) },
	conf("bsautorequest.config"),
	sc( fm ),
	req( *this ),
	feedback( fm ),
	hotkey(*this)
{
}
