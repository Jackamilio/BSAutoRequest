/*****************************************************
 *	C++ code generated with Nana Creator (0.30.0)
 *	GitHub repo: https://github.com/besh81/nana-creator
 *
 * PLEASE EDIT ONLY INSIDE THE TAGS:
 *		//<*tag
 *			user code
 *		//*>
*****************************************************/

#ifndef GEN_SETTINGS_H
#define GEN_SETTINGS_H

#include <nana/gui.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/label.hpp>

//<*includes

//*>


class Gen_settings
	: public nana::form
{
public:
	Gen_settings(nana::window wd, const ::nana::size& sz = {300, 250}, const nana::appearance& apr = {true, false, false, true, false, false, false})
		: nana::form(wd, sz, apr)
	{
		init_();

		//<*ctor

		//*>
	}

	~Gen_settings()
	{
		//<*dtor

		//*>
	}


private:
	void init_()
	{
		_place.bind(*this);
		_place.div("margin=[5,5,5,5] <vert margin=[5,5,5,5] gap=2 arrange=[60,variable] MainLayout>");
		caption("Settings");
		// bsfoldergroup
		bsfoldergroup.create(*this);
		bsfoldergroup.div("margin=[5,5,5,5] gap=2 arrange=[80%,variable] _field_");
		bsfoldergroup.caption("Beat Saber folder");
		_place["MainLayout"] << bsfoldergroup;
		// bsfoldertbox
		bsfoldertbox.create(bsfoldergroup);
		bsfoldergroup["_field_"] << bsfoldertbox;
		bsfoldertbox.multi_lines(false);
		// bsfolderbtn
		bsfolderbtn.create(bsfoldergroup);
		bsfoldergroup["_field_"] << bsfolderbtn;
		bsfolderbtn.caption("Set");
		// hotkeygroup
		hotkeygroup.create(*this);
		hotkeygroup.div("vert margin=[5,5,5,5] gap=2 arrange=[25,variable] _field_");
		hotkeygroup.caption("Hotkey");
		_place["MainLayout"] << hotkeygroup;
		// enablepanel
		enablepanel.create(hotkeygroup);
		enablepanel_place.bind(enablepanel);
		enablepanel_place.div("weight=25 margin=[5,5,5,5] gap=2 arrange=[70,20,variable] _field_");
		hotkeygroup["_field_"] << enablepanel;
		// enablepadding
		enablepadding.create(enablepanel);
		enablepanel_place["_field_"] << enablepadding;
		// enablecbx
		enablecbx.create(enablepanel);
		enablepanel_place["_field_"] << enablecbx;
		enablecbx.caption("");
		// enablelbl
		enablelbl.create(enablepanel);
		enablepanel_place["_field_"] << enablelbl;
		enablelbl.caption("Enable hotkey");
		// keysettings
		keysettings.create(hotkeygroup);
		keysettings_place.bind(keysettings);
		keysettings_place.div("margin=[5,5,5,5] <grid=[1,3] margin=[5,5,5,5] gap=2 modifiersgrid><margin=[5,5,5,5] gap=2 hotkeybtnfield>");
		hotkeygroup["_field_"] << keysettings;
		// ctrlpanel
		ctrlpanel.create(keysettings);
		ctrlpanel_place.bind(ctrlpanel);
		ctrlpanel_place.div("margin=[5,5,5,5] gap=2 arrange=[20,variable] _field_");
		keysettings_place["modifiersgrid"] << ctrlpanel;
		// ctrlcbx
		ctrlcbx.create(ctrlpanel);
		ctrlpanel_place["_field_"] << ctrlcbx;
		ctrlcbx.caption("");
		// ctrllbl
		ctrllbl.create(ctrlpanel);
		ctrlpanel_place["_field_"] << ctrllbl;
		ctrllbl.caption("Control");
		// shiftpanel
		shiftpanel.create(keysettings);
		shiftpanel_place.bind(shiftpanel);
		shiftpanel_place.div("margin=[5,5,5,5] gap=2 arrange=[20,variable] _field_");
		keysettings_place["modifiersgrid"] << shiftpanel;
		// shiftcbx
		shiftcbx.create(shiftpanel);
		shiftpanel_place["_field_"] << shiftcbx;
		shiftcbx.caption("");
		// shiftlbl
		shiftlbl.create(shiftpanel);
		shiftpanel_place["_field_"] << shiftlbl;
		shiftlbl.caption("Shift");
		// altpanel
		altpanel.create(keysettings);
		altpanel_place.bind(altpanel);
		altpanel_place.div("margin=[5,5,5,5] gap=2 arrange=[20,variable] _field_");
		keysettings_place["modifiersgrid"] << altpanel;
		// altcbx
		altcbx.create(altpanel);
		altpanel_place["_field_"] << altcbx;
		altcbx.caption("");
		// altlbl
		altlbl.create(altpanel);
		altpanel_place["_field_"] << altlbl;
		altlbl.caption("Alt");
		// hotkeybtn
		hotkeybtn.create(keysettings);
		keysettings_place["hotkeybtnfield"] << hotkeybtn;
		hotkeybtn.caption("Set key");

		_place.collocate();
		bsfoldergroup.collocate();
		hotkeygroup.collocate();
		enablepanel_place.collocate();
		keysettings_place.collocate();
		ctrlpanel_place.collocate();
		shiftpanel_place.collocate();
		altpanel_place.collocate();
	}


protected:
	nana::place _place;
	nana::group bsfoldergroup;
	nana::textbox bsfoldertbox;
	nana::button bsfolderbtn;
	nana::group hotkeygroup;
	nana::panel<true> enablepanel;
	nana::place enablepanel_place;
	nana::panel<true> enablepadding;
	nana::checkbox enablecbx;
	nana::label enablelbl;
	nana::panel<true> keysettings;
	nana::place keysettings_place;
	nana::panel<true> ctrlpanel;
	nana::place ctrlpanel_place;
	nana::checkbox ctrlcbx;
	nana::label ctrllbl;
	nana::panel<true> shiftpanel;
	nana::place shiftpanel_place;
	nana::checkbox shiftcbx;
	nana::label shiftlbl;
	nana::panel<true> altpanel;
	nana::place altpanel_place;
	nana::checkbox altcbx;
	nana::label altlbl;
	nana::button hotkeybtn;


	//<*declarations

	//*>
};

#endif //GEN_SETTINGS_H

