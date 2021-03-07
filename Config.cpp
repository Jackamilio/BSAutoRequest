#include "Config.h"

#include <fstream>
#include <iostream>

void Config::save() {
    std::ofstream ofs(configpath());
    ofs << std::setw(4) << json << std::endl;
}

Config::Config() {
    // Load the config if its file exists
    if (std::filesystem::exists(configpath())) {
        std::ifstream ifs(configpath());
        json = nlohmann::json::parse(ifs, nullptr, false, false);

        if (json.type() == nlohmann::detail::value_t::discarded) {
            std::cerr << "Error parsing " + configpath().string() << std::endl;
        }
    }
}

void Config::setBSPath(const std::filesystem::path& path) {
    json["beatsaberfolder"] = path.string();
    save();
}

std::filesystem::path Config::getBSPath() const {
    //wtf "value" is useful for? It throws an exception if it doesn't find the key. Not the default value specified. wtf?
    //return json.value("beatsaberfolder", "");
    //I have to ugly try catch this mess now
    try {
        return json["beatsaberfolder"].get<std::string>();
    }
    catch (std::exception e) {
        return "";
    }
}

std::filesystem::path Config::getJsonPath() const {
    return std::filesystem::path(getBSPath().string() + "\\UserData\\BeatSaberPlus_ChatRequestDB.json");
}
