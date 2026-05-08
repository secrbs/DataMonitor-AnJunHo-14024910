#include <iostream>
#include <string>
#include "monitor/JsonFileMonitor.h"

int main(int argc, char* argv[]) {
    std::string dataDir = "data";
    int refreshSeconds = 3;

    if (argc > 1) dataDir = argv[1];
    if (argc > 2) refreshSeconds = std::stoi(argv[2]);

    JsonFileMonitor monitor(dataDir, refreshSeconds);
    monitor.run();
    return 0;
}
