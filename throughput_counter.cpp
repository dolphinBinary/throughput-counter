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
        Stats stat{};
        if (iss >> stat.start >> stat.end >> stat.cellId >> stat.imsi >> stat.rnti >> stat.lcid
                >> stat.nTxPDUs >> stat.txBytes >> stat.nRxPDUs >> stat.rxBytes >> stat.delay
                >> stat.stdDevDelay >> stat.minDelay >> stat.maxDelay >> stat.pduSize
                >> stat.stdDevPduSize >> stat.minPduSize >> stat.maxPduSize) {
            stats.push_back(stat); // TODO: можно улучшить, при копировании можно не амортизировать стоимость, есть проблема, способ решения возомжно надо работать по ссылке, из за неизвестного объёма входных данных, при параллельной работе с файлами проблем возникнуть не должно, если будет очень много строк в файле то скорее всего будут проблемы при копировании
            // TODO: Возможно можно выкинуть чтение не нужной статистики и не записывать не нужные поля
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

    if (totalTime == 0) {
        return 0; // Избегаем деления на ноль
    }

    // Преобразуем байты в биты и время в секунды
    double throughput = (totalBytes * 8) / totalTime;
    return throughput;
}

int main() {
    std::cout << std::fixed << std::setprecision(2);

    std::vector<Stats> dlStats = readStats("../output/DlRlcStats.txt");
    std::vector<Stats> ulStats = readStats("../output/UlRlcStats.txt");

    double dlThroughput = calculateThroughput(dlStats);
    double ulThroughput = calculateThroughput(ulStats);

    std::cout << "Передающая пропускная способность: " << dlThroughput / 1000 << " Кб/с" << std::endl;
    std::cout << "Принимающая пропускная способность: " << ulThroughput / 1000 << " Кб/с" << std::endl;

    return 0;
}

// TODO: подумать над параллелизмом кода, так как тут два абсолютно независимых потока, в зависимости от возможностей станции