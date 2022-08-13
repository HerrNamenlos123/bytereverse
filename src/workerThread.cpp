
#include "pch.h"
#include "workerThread.h"

#include <windows.h>

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

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	si.wShowWindow = SW_HIDE;

	std::string bytereverse = Battery::GetExecutableDirectory() + "/bytereverse.exe";
	std::string cmdline = fmt::format("\"{}\" \"{}\" \"{}\"", bytereverse, sourceFile, targetFile);
	if (!CreateProcessW(NULL, (LPWSTR)Battery::MultiByteToWideChar(cmdline).c_str(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		errorMessage = fmt::format("CreateProcess failed ({})", GetLastError());
		return false;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);

	DWORD exitCode;
	if (!GetExitCodeProcess(pi.hProcess, &exitCode)) {
		errorMessage = fmt::format("GetExitCodeProcess failed ({})", GetLastError());
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return false;
	}
	
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	if (exitCode != 0) {
		errorMessage = "The bytereverse.exe utility failed to process the file. Error code: " + std::to_string(exitCode);
		return false;
	}

	return true;
}
