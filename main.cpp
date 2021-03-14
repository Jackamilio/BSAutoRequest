#include <Windows.h>
#include <nana/config.hpp>
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/paint/graphics.hpp>
#include <nana/gui/filebox.hpp>
#include <nana/gui/timer.hpp>
#include <nana/paint/image.hpp>
#include <nana/gui/notifier.hpp>
#include <nana/gui/widgets/menu.hpp>

#include "resource.h"
#include <string>
#include <iostream>
#include <fstream>

#include "utils.h"

#include "Feedback.h"
#include "Config.h"
#include "Requests.h"
#include "Hotkey.h"
#include "subclass.h"

using namespace nana;

int main()
{
    // Define a form.
    form fm{ API::make_center(300, 200), nana::appearance(true,true,false,false,true,false,false) };
    fm.caption("BS Auto Request");

    std::string apppath = GetAppPath();

    fm.icon(paint::image(apppath));

    // Unsupported events from nana
    subclass sc(fm);

    // Wanted behavior : the close button removes the program from the taskbar and stays in tray
    sc.make_before(WM_CLOSE, [&fm](UINT, WPARAM, LPARAM, LRESULT*) {
        fm.hide();
        return false;
        });

    // Menu that gives the option to quit, replaces the close button. Used by the sys tray
    nana::menu exitmenu;
    exitmenu.append("Exit", [&fm](menu::item_proxy& ip) {
        fm.close();
        });

    // Sys tray icon
    notifier tray(fm);
    tray.icon(apppath);
    tray.text("BS Auto Request");
    tray.events().mouse_up([&fm,&exitmenu](const arg_notifier& arg) {
        // Bring back the window on left click
        if (arg.left_button) {
            fm.bring_top(true);
        }
        // Give the exit menu on close
        else if (arg.right_button) {
            exitmenu.popup(fm, arg.pos.x - fm.pos().x, arg.pos.y - fm.pos().y);
        }
        });

    //Label for user feedback
    Feedback feedback{ fm };
    feedback.format(true);

    Requests requests(feedback);

    //Box to put BSR text
    paint::font bigfont("", 32, detail::font_style(800, false, false, false));
    textbox tbox{ fm };
    tbox.multi_lines(false);
    tbox.typeface(bigfont);
    auto cleartbox = [&tbox] {tbox.select(true); tbox.del(); };

    //Function to add request
    auto tryRequest = [&] {
        if (requests.AddAutoRequest(tbox.text())) {
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
    fm.keyboard_accelerator(accel_key{'v', false, false, true, false}, [&] {
        paste();
    });

    // react to right clic as well, no need for a menu, the user intent is most likely to paste
    tbox.events().mouse_up([&](const arg_mouse& cl) { //click doesn't register right click
        if (cl.button == mouse::right_button) {
            paste();
        }
    });

    // React to hotkey even when minimised
    Hotkey hk(fm,sc);
    hk.Register([&] { paste(); tryRequest(); });

    // react to pressing enter
    tbox.events().key_press([&](const arg_keyboard& kb) {
        if (kb.key == '\r') {
            tryRequest();
        }
    });

    //Define a button and answer the click event.
    button btn{ fm, "Submit" };
    btn.events().click(tryRequest);

    //BS Folder button
    button folderbtn{ fm, "BS Folder" };
    folderbtn.events().click([&] {
            folderbox getfolder{ fm, {folderbox::path_type(requests.config.getBSPath())} };
            getfolder.allow_multi_select(false);
            getfolder.title("Please give Beat Saber root folder");
            std::vector<folderbox::path_type> paths = getfolder.show();
            if (paths.empty()) {
                if (!requests.CheckJSon()) {
                    feedback.Error("Error, no BS folder chosen", true);
                }
            }
            else {
                requests.config.setBSPath(paths[0]);
                if (requests.CheckJSon()) {
                    feedback.Success("BS folder set!");
                }
            }
        });

    //Layout management
    fm.div("vert <weight=24<><><bsfolder>><weight=24><weight=64<><weight=60% input><>><weight=24<><button><>><><weight=32<><weight=80% text><>><>");
    fm["input"] << tbox;
    fm["button"] << btn;
    fm["bsfolder"] << folderbtn;
    fm["text"] << feedback;
    fm.collocate();

    //Show the form
    fm.show();

    //Start to event loop process, it blocks until the form is closed.
    exec();
}