
#include "reverse.h"

static uint8_t reverse(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

ReverserResult reverseFile(const std::string& inputfile, const std::string& outputfile) {

    auto file = b::fs::try_read(inputfile);
    if (!file) {
        return ReverserResult::InputFileReadError;
    }

    std::string result;
    result.reserve(file->size());

    std::stringstream ss(*file);
    std::string token;
    while (std::getline(ss, token, ',')) {
        try {
            int value = std::stoi(token);
            result += std::to_string(reverse(value));
            result += ',';
        } catch (...) {
            return ReverserResult::InvalidFileFormatError;
        }
    }

    if (!result.empty()) {
        result.pop_back();
    }

    if (!b::fs::try_write(outputfile, result)) {
        return ReverserResult::OutputFileWriteError;
    }

    return ReverserResult::Success;
}
