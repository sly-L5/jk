
#include <iostream>
#include <string>
#include <csignal>
#include "compressor.h"

bool interrupted = false;

void signalHandler(int signum) {
    std::cout << "\nОперация прервана пользователем (Ctrl+C)\n";
    interrupted = true;
}

int main(int argc, char* argv[]) {
    std::signal(SIGINT, signalHandler);

    if (argc != 4) {
        std::cerr << "Использование:\n";
        std::cerr << "  myapp a <original_file> <archive_file>\n";
        std::cerr << "  myapp e <archive_file> <original_file>\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];

    if (mode == "a") {
        if (!compressFile(inputFile, outputFile, interrupted)) {
            std::cerr << "Ошибка при архивации файла\n";
            return 1;
        }
    } else if (mode == "e") {
        if (!decompressFile(inputFile, outputFile, interrupted)) {
            std::cerr << "Ошибка при распаковке файла\n";
            return 1;
        }
    } else {
        std::cerr << "Неизвестный режим: " << mode << "\n";
        return 1;
    }

    std::cout << "Готово.\n";
    return 0;
}
