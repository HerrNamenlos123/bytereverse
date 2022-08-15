
#include "version.h"
#include "reverse.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

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
    printf("bytereverse version %d.%d.%d\n", BYTEREVERSE_VERSION_MAJOR, BYTEREVERSE_VERSION_MINOR, BYTEREVERSE_VERSION_PATCH);
}

void printUnknownOptionError(const char* error) {
    printf("Unknown option: %s\n", error);
}

void printUsage() {
    printf("usage: bytereverse [--version] [--help] inputfile outputfile\n");
}

void printHelp() {
    printf("\nbytereverse utility by HerrNamenlos123 for byte-reversing FPGA bitstreams for the\n");
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

    if (!actions.valid) return 3;

    return bytereverse(actions.inputfile, actions.outputfile);
}