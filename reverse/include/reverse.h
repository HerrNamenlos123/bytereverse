#pragma once

#include "battery/core.hpp"

enum class ReverserResult {
    Success,
    InputFileReadError,
    OutputFileWriteError,
    InvalidFileFormatError
};

ReverserResult reverseFile(const std::string& inputfile, const std::string& outputfile);
