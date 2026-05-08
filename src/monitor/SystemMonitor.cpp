#include "SystemMonitor.h"
#include <fstream>

SystemMonitor::SystemMonitor(const std::string& dataDir)
    : dataDir_(dataDir) {}

std::vector<OrderSummary> SystemMonitor::getOrderSummary() const {
    auto orders = loadFile("orders.json");

    std::map<std::string, int> counts;
    for (const std::string& s : {"RESERVED", "CONFIRMED", "PRODUCING", "RELEASE"})
        counts[s] = 0;

    for (const auto& o : orders) {
        std::string status = o.value("status", "");
        if (status == "REJECTED") continue;
        if (counts.count(status)) counts[status]++;
    }

    return {
        {"RESERVED",  counts["RESERVED"]},
        {"CONFIRMED", counts["CONFIRMED"]},
        {"PRODUCING", counts["PRODUCING"]},
        {"RELEASE",   counts["RELEASE"]},
    };
}

std::vector<SampleStockInfo> SystemMonitor::getSampleStockInfo() const {
    auto samples = loadFile("samples.json");
    auto orders  = loadFile("orders.json");

    // 시료별 활성 주문 수량 합산 (RESERVED + CONFIRMED + PRODUCING)
    std::map<std::string, int> pendingMap;
    for (const auto& o : orders) {
        std::string status = o.value("status", "");
        if (status == "REJECTED" || status == "RELEASE") continue;
        std::string sampleId = o.value("sampleId", "");
        pendingMap[sampleId] += o.value("quantity", 0);
    }

    std::vector<SampleStockInfo> result;
    for (const auto& s : samples) {
        std::string id = s.value("id", "");
        int stock      = s.value("stock", 0);
        int pending    = pendingMap.count(id) ? pendingMap[id] : 0;
        result.push_back({id, s.value("name", ""), stock, pending,
                          calcStockStatus(stock, pending)});
    }
    return result;
}

nlohmann::json SystemMonitor::loadFile(const std::string& filename) const {
    std::ifstream f(dataDir_ + "/" + filename);
    if (!f.is_open()) return nlohmann::json::array();
    try {
        nlohmann::json j;
        f >> j;
        return j;
    } catch (...) {
        return nlohmann::json::array();
    }
}

StockStatus SystemMonitor::calcStockStatus(int stock, int pending) const {
    if (stock == 0)            return StockStatus::DEPLETED;
    if (stock < pending)       return StockStatus::SHORT;
    return StockStatus::SURPLUS;
}
