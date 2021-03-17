#pragma once

#include <filesystem>
#include <map>
#include "json.hpp"

class Config {
public:
    enum class StringVars { bsfolderpath };
    enum class NumVars { hotkeycode };
    enum class BoolVars { hotkeyenabled, hotkeyctrl, hotkeyshift, hotkeyalt };

private:
    std::filesystem::path configpath;
    nlohmann::json json;
    bool needsave;

    std::map<StringVars, std::string> strDefaults;
    std::map<NumVars, double> numDefaults;
    std::map<BoolVars, bool> boolDefaults;

    bool setNum(const NumVars nv, double val);
    double getNum(const NumVars nv) const;

public:
    Config(const std::filesystem::path& filename);

    void save();

    // getters
    bool getBool(const BoolVars bv) const;
    std::string getStr(const StringVars sv) const;
    template<typename T>
    T getNumAs(const NumVars nv) const {
        return (T)getNum(nv);
    }

    // setters : returns if the value was different then changed
    bool setStr(const StringVars sv, const std::string& val);
    bool setBool(const BoolVars bv, bool val);
    template<typename T>
    bool setNumAs(const NumVars nv, const T& val) {
        return setNum(nv, (double)val);
    }
};