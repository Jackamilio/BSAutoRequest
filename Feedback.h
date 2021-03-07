#pragma once

#include <string>
#include <nana/gui.hpp>
#include <nana/gui/timer.hpp>
#include <nana/gui/widgets/label.hpp>

class Feedback : public nana::label {
private:
    nana::timer blink;
    std::string currentmsg;
    int blinkcounter;
    bool insist;
    const nana::form& fm;

public:
    Feedback(nana::form& f);

    void Success(const std::string& msg);
    void Error(const std::string& msg, bool critical);
    void InsistOnLastError();
};