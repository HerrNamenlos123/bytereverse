
#include "pch.h"
#include "workerThread.h"

bool doByteReverseWork(std::string& errorMessage, const std::string profileName, const std::string sourceFile, const std::string targetFile) {

	Battery::Sleep(5);

	if (profileName == "") {
		errorMessage = "The chosen profile is invalid, please create one";
		return false;
	}

	return true;
}
