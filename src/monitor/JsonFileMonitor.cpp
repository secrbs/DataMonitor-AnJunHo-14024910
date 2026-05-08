#include "JsonFileMonitor.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <filesystem>
#include <chrono>
#include <thread>
#include <ctime>
#include <conio.h>

namespace fs = std::filesystem;

JsonFileMonitor::JsonFileMonitor(const std::string& dataDir, int refreshSeconds)
    : dataDir_(dataDir), refreshSeconds_(refreshSeconds) {}

void JsonFileMonitor::run() {
    std::cout << "DataMonitor 시작 중... (종료: Q)\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    while (true) {
        drawDashboard();

        // refreshSeconds 동안 Q 입력 감지
        for (int i = 0; i < refreshSeconds_ * 10; ++i) {
            if (_kbhit()) {
                char ch = static_cast<char>(_getch());
                if (ch == 'Q' || ch == 'q') {
                    std::cout << "\n종료합니다.\n";
                    return;
                }
                if (ch == 'r' || ch == 'R') break; // 즉시 새로고침
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

void JsonFileMonitor::drawDashboard() const {
    // 화면 지우기
    system("cls");

    auto files = listJsonFiles();

    std::cout << "╔══════════════════════════════════════════════════╗\n";
    std::cout << "║          DataMonitor  [실시간 데이터 조회]        ║\n";
    std::cout << "╚══════════════════════════════════════════════════╝\n";
    std::cout << "  갱신 시각 : " << currentTimestamp() << "\n";
    std::cout << "  갱신 주기 : " << refreshSeconds_ << "초"
              << "  |  [R] 즉시 갱신  [Q] 종료\n";
    std::cout << "  경로      : " << fs::absolute(dataDir_).string() << "\n";
    std::cout << "\n";

    if (files.empty()) {
        std::cout << "  조회 가능한 JSON 파일이 없습니다.\n";
        return;
    }

    for (const auto& f : files)
        drawFileSection(f);
}

void JsonFileMonitor::drawFileSection(const std::string& filePath) const {
    auto data = loadFile(filePath);
    std::string filename = fs::path(filePath).filename().string();

    std::cout << "┌─ " << filename << " ── " << data.size() << "건 ";
    std::cout << std::string(std::max(0, 48 - (int)filename.size() - 6), '─') << "┐\n";
    printTable(data);
    std::cout << "\n";
}

void JsonFileMonitor::printTable(const nlohmann::json& data) const {
    if (data.empty() || !data[0].is_object()) {
        std::cout << "  (데이터 없음)\n";
        return;
    }

    std::vector<std::string> keys;
    for (auto& [key, _] : data[0].items())
        keys.push_back(key);

    std::vector<size_t> widths;
    for (const auto& k : keys)
        widths.push_back(k.size());

    auto cellVal = [](const nlohmann::json& j, const std::string& key) -> std::string {
        if (!j.contains(key)) return "-";
        std::string v = j[key].dump();
        if (v.size() >= 2 && v.front() == '"' && v.back() == '"')
            v = v.substr(1, v.size() - 2);
        return v;
    };

    for (const auto& row : data)
        for (size_t i = 0; i < keys.size(); ++i)
            widths[i] = std::max(widths[i], cellVal(row, keys[i]).size());

    printSeparator(widths);
    std::cout << "  |";
    for (size_t i = 0; i < keys.size(); ++i)
        std::cout << " " << std::left << std::setw(widths[i]) << keys[i] << " |";
    std::cout << "\n";
    printSeparator(widths);

    for (const auto& row : data) {
        std::cout << "  |";
        for (size_t i = 0; i < keys.size(); ++i)
            std::cout << " " << std::left << std::setw(widths[i]) << cellVal(row, keys[i]) << " |";
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

std::vector<std::string> JsonFileMonitor::listJsonFiles() const {
    std::vector<std::string> files;
    if (!fs::exists(dataDir_) || !fs::is_directory(dataDir_)) return files;
    for (const auto& entry : fs::directory_iterator(dataDir_))
        if (entry.path().extension() == ".json")
            files.push_back(entry.path().string());
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

std::string JsonFileMonitor::currentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
    localtime_s(&tm, &t);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
    return buf;
}
