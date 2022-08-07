
#include "pch.h"
#include "Shortcut.h"

//bool CreateShortcut(const std::string& shortcutLocation, const std::string& shortcutName, const std::string& linkTarget) {
//    auto cmd = fmt::format("cd /D {} && mklink /h {} {}", shortcutLocation, shortcutName, linkTarget);
//    auto[success, returnCode] = Battery::ExecuteShellCommand(cmd);
//    return success ? (returnCode == 0) : false;
//}

#include <windows.h>
#include <shobjidl.h>
#include <shlguid.h>

bool CreateShortcut(const std::string& shortcutPath, const std::string& shortcutTarget) {

    bool success = false;
    IShellLinkW* psl;
    
    if (SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, (LPVOID*)&psl))) {

        psl->SetPath(Battery::MultiByteToWideChar(shortcutTarget).c_str());

        IPersistFile* ppf;
        if (SUCCEEDED(psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf))) {
            success = SUCCEEDED(ppf->Save(Battery::MultiByteToWideChar(shortcutPath).c_str(), TRUE));
            ppf->Release();
        }
        psl->Release();
    }
    return success;
}
