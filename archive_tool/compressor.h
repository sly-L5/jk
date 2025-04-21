
#pragma once

#include <string>

bool compressFile(const std::string& inputFile, const std::string& outputFile, bool& interrupted);
bool decompressFile(const std::string& inputFile, const std::string& outputFile, bool& interrupted);
