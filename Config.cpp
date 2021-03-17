#include "Config.h"

#include <Windows.h>
#include <fstream>
#include <iostream>

#include "magic_enum.hpp"

void Config::save() {
    if (needsave) {
        std::ofstream ofs(configpath);
        ofs << std::setw(4) << json << std::endl;
        needsave = false;
        std::cout << "Saved in the config file." << std::endl;
    }
    else {
        std::cout << "Attempted to save in config but nothing changed, so nothing happened." << std::endl;
    }
}

Config::Config(const std::filesystem::path& filename) : configpath(filename), needsave(false) {
    // Load the config if its file exists
    if (std::filesystem::exists(configpath)) {
        std::ifstream ifs(configpath);
        json = nlohmann::json::parse(ifs, nullptr, false, false);

        if (json.type() == nlohmann::detail::value_t::discarded) {
            std::cerr << "Error parsing " + configpath.string() << std::endl;
        }
    }

    // init defaults
    strDefaults[StringVars::bsfolderpath] = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Beat Saber";

    numDefaults[NumVars::hotkeycode] = (double)int(VK_F5);

    boolDefaults[BoolVars::hotkeyenabled] = true;
    boolDefaults[BoolVars::hotkeyctrl] = true;
    boolDefaults[BoolVars::hotkeyshift] = true;
    boolDefaults[BoolVars::hotkeyalt] = false;
}

template<typename R, typename E>
R getVal(const E e, const nlohmann::json& json, const std::map<E, R>& defaults, R ultimatedefault) {
    const char* key = magic_enum::enum_name(e).data();
    auto find = json.find(key);
    if (find != json.end()) {
        return *find;
    }
    else {
        auto it = defaults.find(e);
        if (it != defaults.end()) {
            return it->second;
        }
    }
    return ultimatedefault;
}

std::string Config::getStr(const Config::StringVars sv) const {
    return getVal(sv, json, strDefaults, std::string(""));
}

double Config::getNum(const Config::NumVars nv) const {
    return getVal(nv, json, numDefaults, 0.0);
}

bool Config::getBool(const Config::BoolVars bv) const {
    return getVal(bv, json, boolDefaults, false);
}

bool Config::setStr(const Config::StringVars sv, const std::string& val) {
    if (getStr(sv) != val) {
        json[magic_enum::enum_name(sv).data()] = val;
        needsave = true;
        return true;
    }
    return false;
}

bool Config::setNum(const Config::NumVars nv, double val) {
    if (getNum(nv) != val) {
        json[magic_enum::enum_name(nv).data()] = val;
        needsave = true;
        return true;
    }
    return false;
}

bool Config::setBool(const Config::BoolVars bv, bool val) {
    if (getBool(bv) != val) {
        json[magic_enum::enum_name(bv).data()] = val;
        needsave = true;
        return true;
    }
    return false;
}
