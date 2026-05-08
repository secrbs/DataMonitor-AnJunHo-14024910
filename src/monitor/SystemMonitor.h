#pragma once
#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

enum class StockStatus { SURPLUS, SHORT, DEPLETED };

struct OrderSummary {
    std::string status;
    int count;
};

struct SampleStockInfo {
    std::string id;
    std::string name;
    int stock;
    int pendingQuantity;  // RESERVED + CONFIRMED + PRODUCING 합산
    StockStatus stockStatus;
};

class SystemMonitor {
public:
    explicit SystemMonitor(const std::string& dataDir);

    std::vector<OrderSummary> getOrderSummary() const;
    std::vector<SampleStockInfo> getSampleStockInfo() const;

private:
    std::string dataDir_;

    nlohmann::json loadFile(const std::string& filename) const;
    StockStatus calcStockStatus(int stock, int pending) const;
};
