#pragma once
#include <string>
#include <vector>

struct Column {
    std::string header;
    size_t width;
};

class TableRenderer {
public:
    void render(const std::vector<Column>& columns,
                const std::vector<std::vector<std::string>>& rows) const;

private:
    void printSeparator(const std::vector<Column>& columns) const;
    void printRow(const std::vector<Column>& columns,
                  const std::vector<std::string>& cells) const;
};
