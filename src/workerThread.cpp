
#include "pch.h"
#include "workerThread.h"

bool doByteReverseWork(std::string& errorMessage, const std::string profileName, const std::string sourceFile, const std::string targetFile) {

	if (profileName == "") {
		errorMessage = "The chosen profile is invalid. Please right-click the tray icon and create one";
		return false;
	}

	if (sourceFile == "") {
		errorMessage = "The input file is invalid. Please right-click the tray icon and choose one";
		return false;
	}

	if (targetFile == "") {
		errorMessage = "The output file is invalid. Please right-click the tray icon and choose one";
		return false;
	}

	Battery::Sleep(5);

	return true;
}
