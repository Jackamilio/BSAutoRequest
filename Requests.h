#pragma once

#include <filesystem>
class Globals;

class Requests {
private:
    Globals& g;
    bool lastcheck;

    std::filesystem::path GetOldBSPlusJsonPath() const;
    std::filesystem::path GetNewBSPlusJsonPath() const;
    std::filesystem::path GetCorrectBSPlusJsonPath() const;

public:

    static void FilterString(std::string& bsr);

    Requests(Globals& globals);

    bool CheckJSon();
    bool AddAutoRequest(const std::string& bsr);
};
