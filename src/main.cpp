#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <thread>
#include <ctime>
#include <conio.h>
#include "monitor/SystemMonitor.h"

std::string timestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
    localtime_s(&tm, &t);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
    return buf;
}

std::string stockLabel(StockStatus s) {
    switch (s) {
        case StockStatus::SURPLUS:  return "[여유]";
        case StockStatus::SHORT:    return "[부족]";
        case StockStatus::DEPLETED: return "[고갈]";
    }
    return "";
}

void draw(const SystemMonitor& monitor) {
    system("cls");

    std::cout << "╔══════════════════════════════════════════════════╗\n";
    std::cout << "║           시스템 모니터링 [실시간 현황]            ║\n";
    std::cout << "╚══════════════════════════════════════════════════╝\n";
    std::cout << "  갱신 시각 : " << timestamp() << "  [R] 즉시갱신  [Q] 종료\n\n";

    // 주문량 확인
    std::cout << "[ 주문 현황 ]\n";
    std::cout << "  +-----------+------+\n";
    std::cout << "  | 상태      |  건수|\n";
    std::cout << "  +-----------+------+\n";
    for (const auto& o : monitor.getOrderSummary())
        std::cout << "  | " << std::left  << std::setw(9) << o.status
                  << " | " << std::right << std::setw(4) << o.count << " |\n";
    std::cout << "  +-----------+------+\n\n";

    // 재고량 확인
    std::cout << "[ 재고 현황 ]\n";
    std::cout << "  +--------+----------------------+--------+----------+--------+\n";
    std::cout << "  | ID     | 시료명               |   재고 |   주문량 | 상태   |\n";
    std::cout << "  +--------+----------------------+--------+----------+--------+\n";
    for (const auto& s : monitor.getSampleStockInfo()) {
        std::cout << "  | " << std::left  << std::setw(6)  << s.id
                  << " | " << std::left  << std::setw(20) << s.name
                  << " | " << std::right << std::setw(6)  << s.stock
                  << " | " << std::right << std::setw(8)  << s.pendingQuantity
                  << " | " << std::left  << std::setw(6)  << stockLabel(s.stockStatus)
                  << " |\n";
    }
    std::cout << "  +--------+----------------------+--------+----------+--------+\n";
}

int main(int argc, char* argv[]) {
    std::string dataDir    = "data";
    int refreshSeconds     = 3;

    if (argc > 1) dataDir       = argv[1];
    if (argc > 2) refreshSeconds = std::stoi(argv[2]);

    SystemMonitor monitor(dataDir);

    while (true) {
        draw(monitor);

        for (int i = 0; i < refreshSeconds * 10; ++i) {
            if (_kbhit()) {
                char ch = static_cast<char>(_getch());
                if (ch == 'Q' || ch == 'q') { std::cout << "\n종료합니다.\n"; return 0; }
                if (ch == 'R' || ch == 'r') break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}
