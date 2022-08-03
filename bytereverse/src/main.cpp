
#include "version.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct Actions_t {
    bool printVersion = false;
    bool printUnknownOptionError = false;
    std::string unknownOptionString;
    bool printUsage = false;
    bool printHelp = false;
    
    std::string inputfile;
    std::string outputfile;
    bool valid = false;
};

void printVersion() {
    printf("bytereverse version %d.%d.%d\n", PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);
}

void printUnknownOptionError(const char* error) {
    printf("Unknown option: %s\n", error);
}

void printUsage() {
    printf("usage: %s [--version] [--help] inputfile outputfile\n", PROJECT_NAME);
}

void printHelp() {
    printf("\nbytereverse utitily by HerrNamenlos123 for byte-reversing FPGA bitstreams for the\n");
    printf("Arduino MKR Vidor 4000. Website: https://github.com/HerrNamenlos123/bytereverse\n\n");
    printUsage();
}

Actions_t parseArguments(const std::vector<std::string>& args) {
    Actions_t actions;

    for (auto arg : args) {
        if (arg == "--version") {
            actions.printVersion = true;
            return actions;
        }
        if (arg == "--help") {
            actions.printHelp = true;
            return actions;
        }
        if (arg[0] == '-') {
            actions.printUnknownOptionError = true;
            actions.unknownOptionString = arg;
            actions.printUsage = true;
            return actions;
        }
        else {
            if (actions.inputfile.empty()) {
                actions.inputfile = arg;
            }
            else {
                actions.outputfile = arg;
            }
        }
    }

    if (args.size() != 2) {
        actions.printUsage = true;
        return actions;
    }

    if (!actions.inputfile.empty() &&
        !actions.outputfile.empty() &&
        !actions.printHelp &&
        !actions.printUnknownOptionError &&
        !actions.printUsage &&
        !actions.printVersion)
    {
        actions.valid = true;
    }

    return actions;
}

char reverse(char b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

bool bytereverse(const std::string& inputfile, const std::string& outputfile) {
    
    std::fstream infile;
    infile.open(inputfile, std::ios::in | std::ios::binary);

    if (infile.fail()) {
        printf("Failed to open input file for reading: %s\n", inputfile.c_str());
        return false;
    }

    std::fstream outfile;
    outfile.open(outputfile, std::ios::out | std::ios::binary);

    if (outfile.fail()) {
        printf("Failed to open output file for writing: %s\n", outputfile.c_str());
        infile.close();
        return false;
    }

    char c;
    while (infile.get(c)) {
        outfile << reverse(c);
    }

    infile.close();
    outfile.close();
    return true;
}

int main(int argc, char** argv) {

    std::vector<std::string> args;
    for (int i = 1; i < argc; i++) {
        args.push_back(std::string(argv[i]));
    }

    Actions_t actions = parseArguments(args);

    if (actions.printHelp) printHelp();
    if (actions.printUnknownOptionError) printUnknownOptionError(actions.unknownOptionString.c_str());
    if (actions.printUsage) printUsage();
    if (actions.printVersion) printVersion();

    if (!actions.valid) return 1;

    bool success = bytereverse(actions.inputfile, actions.outputfile);
    return !success;
}