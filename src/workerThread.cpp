
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

	if (!Battery::FileExists(sourceFile)) {
		errorMessage = "The input file cannot be found: '" + sourceFile + "'";
		return false;
	}

	Battery::PrepareDirectory(Battery::GetParentDirectory(targetFile));

	std::string command = fmt::format("cd /D {} && bytereverse {} {}", Battery::GetExecutableDirectory(), sourceFile, targetFile);
	auto[success, errorCode] = Battery::ExecuteShellCommand(command);

	if (!success) {
		errorMessage = "The bytereverse.exe utility failed to process the file. Error code: " + std::to_string(errorCode);
		return false;
	}

	return true;
}
