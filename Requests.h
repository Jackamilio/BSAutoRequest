#pragma once

#include "Config.h"
class Feedback;

class Requests {
private:
    bool lastcheck;

public:
    Config config;
    Feedback& feedback;

    static void FilterString(std::string& bsr);

    Requests(Feedback& f);

    bool CheckJSon();
    bool AddAutoRequest(const std::string& bsr);
};
