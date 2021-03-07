#pragma once

#include <filesystem>
#include "json.hpp"

class Config {
private:
    nlohmann::json json;

    static const inline std::filesystem::path configpath() {
        return std::filesystem::path{ "bsautorequest.config" };
    }

    void save();

public:
    Config();

    void setBSPath(const std::filesystem::path& path);
    std::filesystem::path getBSPath() const;
    std::filesystem::path getJsonPath() const;
};