
#include "version.h"
#include "reverse.h"
#include "battery/core.hpp"

int b::main(const std::vector<std::string>& args) {

    CLI::App app {
        "bytereverse utility by HerrNamenlos123 for bit-reversing FPGA bitstreams for the\n"
        "Arduino MKR Vidor 4000. Website: https://github.com/HerrNamenlos123/bytereverse"
    };

    b::fs::path inputfile;
    app.add_option("inputfile", inputfile, "Input file")->required()->check(CLI::ExistingFile);

    b::fs::path outputfile;
    app.add_option("outputfile", outputfile, "Output file")->required();

    app.add_flag("--version", [&app](size_t) { 
        b::print("bytereverse utility v{}\n", BYTEREVERSE_VERSION.to_string());
    });

    try {
        app.parse(args.size(), b::args_to_argv(args));
    } 
    catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    return static_cast<int>(reverseFile(inputfile, outputfile));
}
