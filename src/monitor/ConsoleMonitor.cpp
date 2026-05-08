#include "ConsoleMonitor.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <conio.h>

ConsoleMonitor::ConsoleMonitor(int refreshSeconds)
    : refreshSeconds_(refreshSeconds) {}

void ConsoleMonitor::run(DataProvider dataProvider) {
    while (true) {
        draw(dataProvider());

        for (int i = 0; i < refreshSeconds_ * 10; ++i) {
            if (_kbhit()) {
                char ch = static_cast<char>(_getch());
                if (ch == 'Q' || ch == 'q') {
                    std::cout << "\n종료합니다.\n";
                    return;
                }
                if (ch == 'R' || ch == 'r') break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

void ConsoleMonitor::draw(const std::vector<Section>& sections) const {
    system("cls");

    std::cout << "╔══════════════════════════════════════════╗\n";
    std::cout << "║       Console Monitor [실시간 조회]       ║\n";
    std::cout << "╚══════════════════════════════════════════╝\n";
    std::cout << "  갱신 시각 : " << currentTimestamp() << "\n";
    std::cout << "  갱신 주기 : " << refreshSeconds_ << "초"
              << "  [R] 즉시갱신  [Q] 종료\n\n";

    for (const auto& section : sections) {
        std::cout << "[ " << section.title << " ]\n";
        renderer_.render(section.columns, section.rows);
        std::cout << "\n";
    }
}

std::string ConsoleMonitor::currentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
    localtime_s(&tm, &t);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
    return buf;
}
