#pragma once
#include <string>
#include <vector>
#include <functional>
#include "TableRenderer.h"

struct Section {
    std::string title;
    std::vector<Column> columns;
    std::vector<std::vector<std::string>> rows;
};

// 화면에 그릴 데이터를 공급하는 콜백 타입
using DataProvider = std::function<std::vector<Section>()>;

class ConsoleMonitor {
public:
    explicit ConsoleMonitor(int refreshSeconds = 3);

    // dataProvider: 호출될 때마다 최신 Section 목록을 반환
    void run(DataProvider dataProvider);

private:
    int refreshSeconds_;
    TableRenderer renderer_;

    void draw(const std::vector<Section>& sections) const;
    std::string currentTimestamp() const;
};
