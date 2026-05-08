#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "monitor/ConsoleMonitor.h"

namespace fs = std::filesystem;

// JSON 배열 파일을 읽어 Section으로 변환
Section loadSection(const std::string& filePath) {
    Section section;
    section.title = fs::path(filePath).filename().string();

    std::ifstream f(filePath);
    if (!f.is_open()) return section;

    nlohmann::json data;
    try { f >> data; } catch (...) { return section; }

    if (data.empty() || !data[0].is_object()) return section;

    // 첫 번째 객체의 키를 컬럼으로 사용
    for (auto& [key, _] : data[0].items())
        section.columns.push_back({key, key.size()});

    // 각 행의 값 추출 및 컬럼 너비 계산
    for (const auto& row : data) {
        std::vector<std::string> cells;
        for (size_t i = 0; i < section.columns.size(); ++i) {
            std::string val = row.contains(section.columns[i].header)
                ? row[section.columns[i].header].dump() : "-";
            if (val.size() >= 2 && val.front() == '"' && val.back() == '"')
                val = val.substr(1, val.size() - 2);
            section.columns[i].width = std::max(section.columns[i].width, val.size());
            cells.push_back(val);
        }
        section.rows.push_back(cells);
    }

    return section;
}

// data 디렉토리의 JSON 파일 전체를 읽어 Section 목록으로 반환
std::vector<Section> readDataDir(const std::string& dataDir) {
    std::vector<Section> sections;
    if (!fs::exists(dataDir)) return sections;

    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator(dataDir))
        if (entry.path().extension() == ".json")
            files.push_back(entry.path().string());
    std::sort(files.begin(), files.end());

    for (const auto& f : files)
        sections.push_back(loadSection(f));

    return sections;
}

int main(int argc, char* argv[]) {
    std::string dataDir = "data";
    int refreshSeconds = 3;

    if (argc > 1) dataDir = argv[1];
    if (argc > 2) refreshSeconds = std::stoi(argv[2]);

    ConsoleMonitor monitor(refreshSeconds);
    monitor.run([&]() { return readDataDir(dataDir); });

    return 0;
}
