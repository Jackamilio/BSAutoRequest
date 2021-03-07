#include "Feedback.h"
#include "utils.h"

Feedback::Feedback(nana::form& f) : label(f), blink(std::chrono::milliseconds{125}), blinkcounter(0), insist(false), fm(f) {
    blink.elapse([=] {
        blinkcounter += 1;

        if (blinkcounter < 16 || blinkcounter % 2 == 1) {
            caption(currentmsg);
        }
        else {
            caption("");
        }

        if (insist) {
            if (blinkcounter == 33) {
                blink.stop();
            }
        }
        else {
            if (blinkcounter == 32) {
                blink.stop();
            }
        }
        });
}

void Feedback::Success(const std::string& msg) {
    currentmsg = "<bold green size = 16>" + msg + "< / >";
    blinkcounter = 0;
    insist = false;
    blink.start();

    if (!nana::API::focus_window()) {
        TrayMessage(fm.native_handle(), "Success!", msg);
    }
}

void Feedback::Error(const std::string& msg, bool critical) {
    currentmsg = "<bold red>" + msg + "< / >";
    if (critical) {
        caption(currentmsg);
    }
    else {
        blinkcounter = 0;
        insist = false;
        blink.start();
    }

    if (!nana::API::focus_window()) {
        TrayMessage(fm.native_handle(), "Error", msg);
    }
}

void Feedback::InsistOnLastError() {
    blinkcounter = 16;
    insist = true;
    blink.start();
}