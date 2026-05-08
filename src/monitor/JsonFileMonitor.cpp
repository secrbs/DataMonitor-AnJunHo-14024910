#include "JsonFileMonitor.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

JsonFileMonitor::JsonFileMonitor(const std::string& dataDir)
    : dataDir_(dataDir) {}

void JsonFileMonitor::run() {
    while (true) {
        auto files = listJsonFiles();

        std::cout << "\n========================================\n";
        std::cout << "  DataMonitor  [데이터 모니터링 Tool]\n";
        std::cout << "  경로: " << fs::absolute(dataDir_) << "\n";
        std::cout << "========================================\n";

        if (files.empty()) {
            std::cout << "  조회 가능한 JSON 파일이 없습니다.\n";
            std::cout << "\n[R] 새로고침  [Q] 종료 > ";
        } else {
            std::cout << "  JSON 파일 목록\n";
            std::cout << "----------------------------------------\n";
            for (size_t i = 0; i < files.size(); ++i)
                std::cout << "  [" << (i + 1) << "] " << fs::path(files[i]).filename().string() << "\n";
            std::cout << "----------------------------------------\n";
            std::cout << "  [R] 새로고침  [Q] 종료\n";
            std::cout << "선택 > ";
        }

        std::string input;
        std::cin >> input;

        if (input == "Q" || input == "q") {
            std::cout << "종료합니다.\n";
            break;
        }
        if (input == "R" || input == "r") continue;

        try {
            int idx = std::stoi(input);
            if (idx >= 1 && idx <= static_cast<int>(files.size())) {
                displayFile(files[idx - 1]);
                std::cout << "\n[Enter] 메뉴로 돌아가기...";
                std::cin.ignore();
                std::cin.get();
            }
        } catch (...) {
            std::cout << "올바른 번호를 입력하세요.\n";
        }
    }
}

std::vector<std::string> JsonFileMonitor::listJsonFiles() const {
    std::vector<std::string> files;
    if (!fs::exists(dataDir_) || !fs::is_directory(dataDir_)) return files;
    for (const auto& entry : fs::directory_iterator(dataDir_)) {
        if (entry.path().extension() == ".json")
            files.push_back(entry.path().string());
    }
    std::sort(files.begin(), files.end());
    return files;
}

nlohmann::json JsonFileMonitor::loadFile(const std::string& filePath) const {
    std::ifstream file(filePath);
    if (!file.is_open()) return nlohmann::json::array();
    try {
        nlohmann::json j;
        file >> j;
        return j;
    } catch (...) {
        return nlohmann::json::array();
    }
}

void JsonFileMonitor::displayFile(const std::string& filePath) const {
    auto data = loadFile(filePath);
    std::cout << "\n";
    printSummary(filePath, data);
    std::cout << "\n";
    printTable(data);
}

void JsonFileMonitor::printSummary(const std::string& filePath, const nlohmann::json& data) const {
    std::cout << "  파일  : " << fs::path(filePath).filename().string() << "\n";
    std::cout << "  레코드: " << data.size() << "건\n";

    // 필드별 null/비어있는 값 통계
    if (!data.empty() && data[0].is_object()) {
        std::cout << "  필드  :";
        for (auto& [key, _] : data[0].items())
            std::cout << " [" << key << "]";
        std::cout << "\n";
    }
}

void JsonFileMonitor::printTable(const nlohmann::json& data) const {
    if (data.empty() || !data[0].is_object()) {
        std::cout << "  (데이터 없음)\n";
        return;
    }

    // 컬럼 키 수집
    std::vector<std::string> keys;
    for (auto& [key, _] : data[0].items())
        keys.push_back(key);

    // 각 컬럼 최대 너비 계산
    std::vector<size_t> widths;
    for (const auto& k : keys)
        widths.push_back(k.size());

    for (const auto& row : data) {
        for (size_t i = 0; i < keys.size(); ++i) {
            std::string val = row.contains(keys[i]) ? row[keys[i]].dump() : "-";
            // dump()의 따옴표 제거
            if (val.size() >= 2 && val.front() == '"' && val.back() == '"')
                val = val.substr(1, val.size() - 2);
            widths[i] = std::max(widths[i], val.size());
        }
    }

    // 헤더 출력
    printSeparator(widths);
    std::cout << "  |";
    for (size_t i = 0; i < keys.size(); ++i)
        std::cout << " " << std::left << std::setw(widths[i]) << keys[i] << " |";
    std::cout << "\n";
    printSeparator(widths);

    // 데이터 행 출력
    for (const auto& row : data) {
        std::cout << "  |";
        for (size_t i = 0; i < keys.size(); ++i) {
            std::string val = row.contains(keys[i]) ? row[keys[i]].dump() : "-";
            if (val.size() >= 2 && val.front() == '"' && val.back() == '"')
                val = val.substr(1, val.size() - 2);
            std::cout << " " << std::left << std::setw(widths[i]) << val << " |";
        }
        std::cout << "\n";
    }
    printSeparator(widths);
}

void JsonFileMonitor::printSeparator(const std::vector<size_t>& widths) const {
    std::cout << "  +";
    for (auto w : widths)
        std::cout << std::string(w + 2, '-') << "+";
    std::cout << "\n";
}
