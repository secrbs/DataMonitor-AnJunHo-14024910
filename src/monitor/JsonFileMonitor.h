#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class JsonFileMonitor {
public:
    explicit JsonFileMonitor(const std::string& dataDir);
    void run();

private:
    std::string dataDir_;

    std::vector<std::string> listJsonFiles() const;
    nlohmann::json loadFile(const std::string& filePath) const;
    void displayFile(const std::string& filePath) const;
    void printTable(const nlohmann::json& data) const;
    void printSummary(const std::string& filePath, const nlohmann::json& data) const;
    void printSeparator(const std::vector<size_t>& widths) const;
};
