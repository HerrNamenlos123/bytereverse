#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <vector>

static uint8_t reverse(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

static std::vector<int> integerize(const std::string& s, const std::string& del) {
    std::vector<int> ints;
    ints.reserve(s.size() / 4);     // estimate of how many ints will be parsed

    char* str = (char*)s.c_str();
    char* token = strtok(str, ",");
    while (token != nullptr) {
        ints.push_back(atoi(token));
        token = strtok(NULL, ",");
    }

    return ints;
}

static int bytereverse(const std::string& inputfile, const std::string& outputfile) {

    std::fstream infile;
    infile.open(inputfile, std::ios::in | std::ios::binary);

    if (infile.fail()) {
        printf("Failed to open input file for reading: %s\n", inputfile.c_str());
        return 1;
    }

    std::fstream outfile;
    outfile.open(outputfile, std::ios::out | std::ios::binary);

    if (outfile.fail()) {
        printf("Failed to open output file for writing: %s\n", outputfile.c_str());
        infile.close();
        return 2;
    }

    std::stringstream _raw;
    _raw << infile.rdbuf();
    std::string raw = _raw.str();
    std::string output;
    output.resize(raw.size(), 0);
    char* ptr = &output[0];

    auto v = integerize(raw, ",");
    size_t cnt = 0;
    for (int i : v) {
        ptr += snprintf(ptr, output.size(), "%3d,%s", (uint16_t)reverse(i), (cnt % 16 == 15) ? "\n" : "");
        cnt++;
    }

    outfile << output.c_str();

    infile.close();
    outfile.close();
    return 0;
}
