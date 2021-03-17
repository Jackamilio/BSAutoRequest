#include "Settings.h"
#include <nana/gui/filebox.hpp>
#include <Windows.h>

Settings::Settings(Globals& globals) :
    Gen_settings{ globals.fm },
    g(globals),
    presskeytimer(std::chrono::milliseconds{ 1000 }),
    countdown(0)
{

    // retrieve bs folder path from config
    bsfoldertbox.append(g.conf.getStr(Config::StringVars::bsfolderpath), false);

    // react to button to set the bs folder path
    bsfolderbtn.events().click([=] {
        nana::folderbox getfolder{ g.fm, {nana::folderbox::path_type(g.conf.getStr(Config::StringVars::bsfolderpath))} };
        getfolder.allow_multi_select(false);
        getfolder.title("Please give Beat Saber root folder");
        std::vector<nana::folderbox::path_type> paths = getfolder.show();
        if (paths.empty()) {
            if (!g.req.CheckJSon()) {
                g.feedback.Error("Error, no BS folder chosen", true);
            }
        }
        else {
            bsfoldertbox.select(true);
            bsfoldertbox.del();
            bsfoldertbox.append(paths[0], false);
        }
        });

    // hotkey enabled setting
    bool hkenabled = g.conf.getBool(Config::BoolVars::hotkeyenabled);
    enablecbx.check(hkenabled);
    if (!hkenabled) {
        keysettings.hide();
    }
    enablecbx.events().checked([=]() {
        g.conf.setBool(Config::BoolVars::hotkeyenabled, enablecbx.checked());
        enablecbx.checked() ? keysettings.show() : keysettings.hide();
        });

    // hotkey ctrl shift and alt setting
    ctrlcbx.check(g.conf.getBool(Config::BoolVars::hotkeyctrl));
    shiftcbx.check(g.conf.getBool(Config::BoolVars::hotkeyshift));
    altcbx.check(g.conf.getBool(Config::BoolVars::hotkeyalt));

    // ctrl shift and alt combox box event reaction
    ctrlcbx.events().checked([=]() {g.conf.setBool(Config::BoolVars::hotkeyctrl, ctrlcbx.checked()); });
    shiftcbx.events().checked([=]() {g.conf.setBool(Config::BoolVars::hotkeyshift, shiftcbx.checked()); });
    altcbx.events().checked([=]() {g.conf.setBool(Config::BoolVars::hotkeyalt, altcbx.checked()); });

    // handle the button to get the hotkey key
    hotkeybtn.events().click([=] {
        if (countdown <= 0) {
            countdown = 2;
            hotkeybtn.caption("Press key... 3");
            presskeytimer.start();
        }
        else {
            countdown = 0;
            SetHotkeyCaption();
            presskeytimer.stop();
        }
        });

    presskeytimer.elapse([=] {
        hotkeybtn.caption("Press key... " + std::to_string(countdown));
        if (--countdown < 0) {
            SetHotkeyCaption();
            presskeytimer.stop();
        }
        });

    hotkeybtn.events().key_press([=](const nana::arg_keyboard& arg) {
        if (presskeytimer.started() && arg.key >= VK_SPACE && arg.key <= VK_F24) {
            countdown = 0;
            g.conf.setNumAs<int>(Config::NumVars::hotkeycode, (int)arg.key);
            SetHotkeyCaption();
            presskeytimer.stop();
        }
        });

    SetHotkeyCaption();

    // save the config when the settings are closed
    events().destroy([=] {

        std::string folder;
        bsfoldertbox.getline(0, folder);
        if (g.conf.setStr(Config::StringVars::bsfolderpath, folder) && g.req.CheckJSon()) {
            g.feedback.Success("BS folder set!");
        }

        g.hotkey.RegisterFromConfig();

        g.conf.save();
        });
}

void Settings::SetHotkeyCaption() {
    int keycode = g.conf.getNumAs<int>(Config::NumVars::hotkeycode);

    if (keycode >= 0x30 && keycode <= 0x39) {
        hotkeybtn.caption(std::string(1, (char)keycode));
        return;
    }
    else if (keycode >= 0x41 && keycode <= 0x5A) {
        hotkeybtn.caption(std::string(1, (char)keycode));
        return;
    }
    else if (keycode >= 0x60 && keycode <= 0x69) {
        hotkeybtn.caption("Numpad " + std::to_string(keycode-0x60));
        return;
    }
    else if (keycode >= 0x70 && keycode <= 0x87) {
        hotkeybtn.caption("F" + std::to_string(keycode - 0x70 + 1));
        return;
    }

    switch (keycode) {
    case VK_SPACE: hotkeybtn.caption("space"); break;
    case VK_PRIOR: hotkeybtn.caption("pg. up"); break;
    case VK_NEXT: hotkeybtn.caption("pg. down"); break;
    case VK_END: hotkeybtn.caption("end"); break;
    case VK_HOME: hotkeybtn.caption("home"); break;
    case VK_LEFT: hotkeybtn.caption("left"); break;
    case VK_UP: hotkeybtn.caption("up"); break;
    case VK_RIGHT: hotkeybtn.caption("right"); break;
    case VK_DOWN: hotkeybtn.caption("down"); break;
    case VK_SELECT: hotkeybtn.caption("select"); break;
    case VK_PRINT: hotkeybtn.caption("print"); break;
    case VK_EXECUTE: hotkeybtn.caption("execute"); break;
    case VK_SNAPSHOT: hotkeybtn.caption("print screen"); break;
    case VK_INSERT: hotkeybtn.caption("insert"); break;
    case VK_DELETE: hotkeybtn.caption("del"); break;
    case VK_HELP: hotkeybtn.caption("help"); break;
    case VK_LWIN: hotkeybtn.caption("win L"); break;
    case VK_RWIN: hotkeybtn.caption("win R"); break;
    case VK_APPS: hotkeybtn.caption("apps"); break;
    case VK_SLEEP: hotkeybtn.caption("sleep"); break;
    case VK_MULTIPLY: hotkeybtn.caption("*"); break;
    case VK_ADD: hotkeybtn.caption("+"); break;
    case VK_SEPARATOR: hotkeybtn.caption("separator"); break;
    case VK_SUBTRACT: hotkeybtn.caption("-"); break;
    case VK_DECIMAL: hotkeybtn.caption("decimal"); break;
    case VK_DIVIDE: hotkeybtn.caption("/"); break;
    default: hotkeybtn.caption("undefined key"); break;
    }
}