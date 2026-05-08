#include <iostream>
#include "monitor/JsonFileMonitor.h"

int main(int argc, char* argv[]) {
    std::string dataDir = "data";
    if (argc > 1) dataDir = argv[1];

    JsonFileMonitor monitor(dataDir);
    monitor.run();
    return 0;
}
