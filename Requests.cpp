#include "Requests.h"
#include "Globals.h"

#include "utils.h"
#include <fstream>

std::filesystem::path Requests::GetBSPlusJsonPath() const {
    return std::filesystem::path(g.conf.getStr(Config::StringVars::bsfolderpath) + "\\UserData\\BeatSaberPlus_ChatRequestDB.json");
}

//removes all the text around the bsr code likely to be put there
void Requests::FilterString(std::string& bsr) {
    // lowercase the bsr first
    std::transform(bsr.begin(), bsr.end(), bsr.begin(), ::tolower);

    // remove "!bsr" if any
    std::istringstream iss(bsr);
    iss >> bsr;
    if (bsr == "!bsr" || bsr == "bsr") {
        iss >> bsr;
    }

    // remove anything before the last "/" character, except if its THE last character
    // this should handle any web links ending with a bsr
    if (bsr.size() > 0 && bsr.at(bsr.size() - 1) == '/') {
        bsr = bsr.substr(0, bsr.size() - 1);
    }

    size_t last = bsr.find_last_of('/');
    if (last != std::string::npos) {
        bsr = bsr.substr(last + 1, bsr.size() - last - 1);
    }
}

Requests::Requests(Globals& globals) : g(globals), lastcheck(false) {
    CheckJSon();
}

bool Requests::CheckJSon() {
    lastcheck = false;

    // first check if the folder exists
    if (!std::filesystem::exists(g.conf.getStr(Config::StringVars::bsfolderpath))) {
        g.feedback.Error("Please set BS folder first", true);
        return false;
    }

    // then check if the file exists
    if (!std::filesystem::exists(GetBSPlusJsonPath())) {
        g.feedback.Error("BeatSaberPlus_ChatRequestDB.json not found", true);
        return false;
    }

    lastcheck = true;
    return true;
}

bool Requests::AddAutoRequest(const std::string& bsr) {
    if (bsr.empty()) {
        // request empty : no reaction, no error
        return false;
    }

    if (lastcheck) {
        // parse the json
        std::ifstream ifs(GetBSPlusJsonPath());
        nlohmann::json json = nlohmann::json::parse(ifs, nullptr, false, false);

        if (json.type() == nlohmann::detail::value_t::discarded) {
            g.feedback.Error("Error parsing BeatSaberPlus_ChatRequestDB.json", true);
            return false;
        }

        // filter the bsr
        std::string fbsr(bsr);
        FilterString(fbsr);

        // check the string to be hex characters
        if (!IsStringHex(fbsr)) {
            g.feedback.Error("BSR invalid, " + fbsr + " is not a hex value", false);
            return false;
        }

        // add the request into the parsed json
        json["queue"].push_back(
            {
                {"key", fbsr},
                //{"hash", ""},
                {"rqt", std::time(nullptr)},
                {"rqn", "Yourself"},
                {"npr", "<>"},
                {"msg", ""}
            });

        // save the new json
        std::ofstream ofs(GetBSPlusJsonPath());
        ofs << std::setw(4) << json << std::endl;

        // success!
        g.feedback.Success("BSR " + fbsr + " added!");
        return true;
    }
    else {
        g.feedback.InsistOnLastError();
        return false;
    }
}
