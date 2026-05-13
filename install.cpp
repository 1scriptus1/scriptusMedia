#include <windows.h>

void RegisterApp()
{
    HKEY hKey;

    const char* appPath = "C:\\Program Files\\scriptusMedia\\scriptusMedia.exe";

    // Register application
    RegCreateKeyExA(
        HKEY_CLASSES_ROOT,
        "Applications\\scriptusMedia.exe\\shell\\open\\command",
        0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL
    );

    std::string command = std::string("\"") + appPath + "\" \"%1\"";

    RegSetValueExA(
        hKey,
        NULL,
        0,
        REG_SZ,
        (const BYTE*)command.c_str(),
        command.size() + 1
    );

    RegCloseKey(hKey);
}