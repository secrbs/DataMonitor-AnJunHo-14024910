#include "TableRenderer.h"
#include <iostream>
#include <iomanip>

void TableRenderer::render(const std::vector<Column>& columns,
                           const std::vector<std::vector<std::string>>& rows) const {
    if (columns.empty()) return;

    printSeparator(columns);
    printRow(columns, [&] {
        std::vector<std::string> headers;
        for (const auto& c : columns) headers.push_back(c.header);
        return headers;
    }());
    printSeparator(columns);

    if (rows.empty()) {
        std::cout << "  (데이터 없음)\n";
    } else {
        for (const auto& row : rows)
            printRow(columns, row);
    }

    printSeparator(columns);
    std::cout << "  총 " << rows.size() << "건\n";
}

void TableRenderer::printSeparator(const std::vector<Column>& columns) const {
    std::cout << "  +";
    for (const auto& col : columns)
        std::cout << std::string(col.width + 2, '-') << "+";
    std::cout << "\n";
}

void TableRenderer::printRow(const std::vector<Column>& columns,
                              const std::vector<std::string>& cells) const {
    std::cout << "  |";
    for (size_t i = 0; i < columns.size(); ++i) {
        std::string cell = (i < cells.size()) ? cells[i] : "";
        std::cout << " " << std::left << std::setw(columns[i].width) << cell << " |";
    }
    std::cout << "\n";
}
