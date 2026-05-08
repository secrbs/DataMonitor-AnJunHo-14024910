#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class JsonFileMonitor {
public:
    explicit JsonFileMonitor(const std::string& dataDir, int refreshSeconds = 3);
    void run();

private:
    std::string dataDir_;
    int refreshSeconds_;

    std::vector<std::string> listJsonFiles() const;
    nlohmann::json loadFile(const std::string& filePath) const;

    void drawDashboard() const;
    void drawFileSection(const std::string& filePath) const;
    void printTable(const nlohmann::json& data) const;
    void printSeparator(const std::vector<size_t>& widths) const;

    std::string currentTimestamp() const;
};
