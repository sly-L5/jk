
#include "compressor.h"
#include <zlib.h>
#include <fstream>
#include <iostream>
#include <vector>

constexpr size_t BUFFER_SIZE = 1024 * 1024; // 1 MB

bool compressFile(const std::string& inputFile, const std::string& outputFile, bool& interrupted) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in) {
        std::cerr << "Не удалось открыть входной файл: " << inputFile << "\n";
        return false;
    }

    gzFile out = gzopen(outputFile.c_str(), "wb");
    if (!out) {
        std::cerr << "Не удалось открыть выходной файл: " << outputFile << "\n";
        return false;
    }

    std::vector<char> buffer(BUFFER_SIZE);
    while (!in.eof()) {
        if (interrupted) break;

        in.read(buffer.data(), buffer.size());
        std::streamsize bytesRead = in.gcount();

        if (gzwrite(out, buffer.data(), static_cast<unsigned>(bytesRead)) != bytesRead) {
            std::cerr << "Ошибка записи в архив\n";
            gzclose(out);
            return false;
        }
    }

    gzclose(out);
    return !interrupted;
}

bool decompressFile(const std::string& inputFile, const std::string& outputFile, bool& interrupted) {
    gzFile in = gzopen(inputFile.c_str(), "rb");
    if (!in) {
        std::cerr << "Не удалось открыть архив: " << inputFile << "\n";
        return false;
    }

    std::ofstream out(outputFile, std::ios::binary);
    if (!out) {
        std::cerr << "Не удалось создать выходной файл: " << outputFile << "\n";
        gzclose(in);
        return false;
    }

    std::vector<char> buffer(BUFFER_SIZE);
    int bytesRead = 0;

    while ((bytesRead = gzread(in, buffer.data(), buffer.size())) > 0) {
        if (interrupted) break;
        out.write(buffer.data(), bytesRead);
    }

    gzclose(in);
    return !interrupted;
}
