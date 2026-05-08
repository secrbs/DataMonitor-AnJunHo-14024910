#include <iostream>
#include <vector>
#include <string>
#include "monitor/ConsoleMonitor.h"

// 데이터 공급 예시 — 실제 앱에서는 Repository 등에서 읽어와 Section 으로 변환
std::vector<Section> provideDummyData() {
    Section samples;
    samples.title = "시료 목록";
    samples.columns = {
        {"ID",       6},
        {"이름",    20},
        {"수율",     6},
        {"재고",     6},
    };
    samples.rows = {
        {"S-001", "실리콘 웨이퍼-8인치",  "0.92", "480"},
        {"S-002", "GaN 에피택셜-4인치",   "0.78", "220"},
        {"S-003", "SiC 파워기판-6인치",   "0.92",  "30"},
    };

    Section orders;
    orders.title = "주문 현황";
    orders.columns = {
        {"주문번호",       18},
        {"고객",          14},
        {"시료",          20},
        {"수량",           6},
        {"상태",          10},
    };
    orders.rows = {
        {"ORD-20260508-0001", "삼성전자 파운드리", "SiC 파워기판-6인치", "200", "RESERVED"},
        {"ORD-20260508-0002", "SK하이닉스",       "실리콘 웨이퍼-8인치","150", "CONFIRMED"},
        {"ORD-20260508-0003", "LG이노텍",         "GaN 에피택셜-4인치", "300", "PRODUCING"},
    };

    return {samples, orders};
}

int main(int argc, char* argv[]) {
    int refreshSeconds = 3;
    if (argc > 1) refreshSeconds = std::stoi(argv[1]);

    ConsoleMonitor monitor(refreshSeconds);
    monitor.run(provideDummyData);
    return 0;
}
