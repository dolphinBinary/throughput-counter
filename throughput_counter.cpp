#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

struct Stats {
    int cellId, imsi, rnti, lcid, nTxPDUs, txBytes, nRxPDUs, rxBytes;
    double start, end, delay, stdDevDelay, minDelay, maxDelay, pduSize, stdDevPduSize, minPduSize, maxPduSize;
};

std::vector<Stats> readStats(const std::string& filename) {
    std::vector<Stats> stats;
    std::ifstream file(filename);
    std::string line;

    // Пропускаем строку заголовка
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Stats stat;
        if (iss >> stat.start >> stat.end >> stat.cellId >> stat.imsi >> stat.rnti >> stat.lcid
                >> stat.nTxPDUs >> stat.txBytes >> stat.nRxPDUs >> stat.rxBytes >> stat.delay
                >> stat.stdDevDelay >> stat.minDelay >> stat.maxDelay >> stat.pduSize
                >> stat.stdDevPduSize >> stat.minPduSize >> stat.maxPduSize) {
            stats.push_back(stat);
        } else {
            std::cerr << "Ошибка чтения строки: " << line << std::endl;
        }
    }

    return stats;
}

double calculateThroughput(const std::vector<Stats>& stats) {
    double totalBytes = 0;
    double totalTime = 0;

    for (const auto& stat : stats) {
        totalBytes += stat.txBytes + stat.rxBytes;
        totalTime += (stat.end - stat.start);
    }

    std::cout << "Общее количество байтов: " << totalBytes << std::endl;
    std::cout << "Общее время: " << totalTime << std::endl;

    if (totalTime == 0) {
        return 0; // Избегаем деления на ноль
    }

    // Преобразуем байты в биты и время в секунды
    double throughput = (totalBytes * 8) / totalTime;
    return throughput;
}

int main() {
    std::vector<Stats> dlStats = readStats("../output/DlRlcStats.txt");
    std::vector<Stats> ulStats = readStats("../output/UlRlcStats.txt");

    double dlThroughput = calculateThroughput(dlStats);
    double ulThroughput = calculateThroughput(ulStats);

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "Downlink Throughput: " << dlThroughput / 1000 << " Kbps" << std::endl;
    std::cout << "Uplink Throughput: " << ulThroughput / 1000 << " Kbps" << std::endl;

    return 0;
}
