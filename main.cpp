#include <Windows.h>
#include <nana/config.hpp>
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/paint/graphics.hpp>
#include <nana/gui/timer.hpp>
#include <nana/paint/image.hpp>
#include <nana/gui/notifier.hpp>
#include <nana/gui/widgets/menu.hpp>

#include "resource.h"
#include <string>
#include <iostream>
#include <fstream>

#include "utils.h"

#include "Globals.h"
#include "HotkeyTemplate.h"
#include "Settings.h"

using namespace nana;

int main()
{
    Globals g;
    // Define a form.
    g.fm.caption("BS Auto Request");

    std::string apppath = GetAppPath();

    g.fm.icon(paint::image(apppath));

    /*
    * OLD, people complained.
    // (Wanted behavior : the close button removes the program from the taskbar and stays in tray)
    g.sc.make_before(WM_CLOSE, [&g](UINT, WPARAM, LPARAM, LRESULT*) {
        g.fm.hide();
        return false;
        });
    */
    // New wanted behavior. Close actually CLOSES. Minimize goes to tray.
    g.sc.make_before(WM_SYSCOMMAND, [&g](UINT nID, WPARAM wp, LPARAM lp, LRESULT*) {
        if (wp == SC_MINIMIZE) {
            g.fm.hide();
            return false;
        }
        else {
            return true;
        }
        });

    // Menu that gives the option to quit, replaces the close button. Used by the sys tray
    nana::menu exitmenu;
    exitmenu.append("Exit", [&g](menu::item_proxy& ip) {
        g.fm.close();
        });

    // Sys tray icon
    notifier tray(g.fm);
    tray.icon(apppath);
    tray.text("BS Auto Request");
    tray.events().mouse_up([&g,&exitmenu](const arg_notifier& arg) {
        // Bring back the window on left click
        if (arg.left_button) {
            g.fm.bring_top(true);
        }
        // Give the exit menu on close
        else if (arg.right_button) {
            exitmenu.popup(g.fm, arg.pos.x - g.fm.pos().x, arg.pos.y - g.fm.pos().y);
        }
        });

    //Label for user feedback
    g.feedback.format(true);

    //Box to put BSR text
    paint::font bigfont("", 32, detail::font_style(800, false, false, false));
    textbox tbox{ g.fm };
    tbox.multi_lines(false);
    tbox.typeface(bigfont);
    auto cleartbox = [&tbox] {tbox.select(true); tbox.del(); };

    //Function to add request
    auto tryRequest = [&] {
        if (g.req.AddAutoRequest(tbox.text())) {
            cleartbox();
        }
    };

    auto paste = [&tbox,&cleartbox]() {
        // remove what was there first
        cleartbox();
        // paste clipboard
        tbox.paste();
        // filter the string
        std::string filtered = tbox.text();
        Requests::FilterString(filtered);
        // replace the content with the filtered string
        cleartbox();
        tbox.append(filtered, false);
    };

    // react to ctrl+v globally, even if the tbox doesn't have focus
    g.fm.keyboard_accelerator(accel_key{'v', false, false, true, false}, [&] {
        paste();
    });

    // react to right clic as well, no need for a menu, the user intent is most likely to paste
    tbox.events().mouse_up([&](const arg_mouse& cl) { //click doesn't register right click
        if (cl.button == mouse::right_button) {
            paste();
        }
    });

    // React to hotkey even when minimised
    Hotkey hk(g);
    hk.SetLambda([&] { paste(); tryRequest(); });
    hk.RegisterFromConfig();

    // react to pressing enter
    tbox.events().key_press([&](const arg_keyboard& kb) {
        if (kb.key == '\r') {
            tryRequest();
        }
    });

    //Define a button and answer the click event.
    button btn{ g.fm, "Submit" };
    btn.events().click(tryRequest);

    // Settings button
    button settingsbtn{ g.fm, "Settings" };
    //Settings settings(g);
    settingsbtn.events().click([&g]() {
        form& fm = form_loader<Settings>()(g);
        fm.show();
        API::modal_window(fm);
        });

    //Layout management
    g.fm.div("vert <weight=24<><><settings>><weight=24><weight=64<><weight=60% input><>><weight=24<><button><>><><weight=32<><weight=80% text><>><>");
    g.fm["input"] << tbox;
    g.fm["button"] << btn;
    g.fm["settings"] << settingsbtn;
    g.fm["text"] << g.feedback;
    g.fm.collocate();

    //Show the form
    g.fm.show();

    //Start to event loop process, it blocks until the form is closed.
    exec();
}