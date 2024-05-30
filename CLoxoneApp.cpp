#include "CLoxoneApp.h"
#include <string>
#include "stdafx.h"

#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <iostream>

int CLoxoneApp::getRunningLoxoneApps()
{
    const std::wstring target1 = L"Loxone";
    const std::wstring target2 = L"Loxone.exe";
    int count = 0;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        qDebug() << "Error: could not create process snapshot.\n";
        return 1;
    }
    
    PROCESSENTRY32 process;
    process.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(snapshot, &process)) {
        qDebug() << "Error: could not retrieve first process.\n";
        CloseHandle(snapshot);
        return 1;
    }

    do {
        // Convert process.szExeFile to std::wstring before comparison
        std::string exeFileA = process.szExeFile;
        std::wstring exeFileW(exeFileA.begin(), exeFileA.end());

        if (exeFileW == target1 || exeFileW == target2) {
            count++;
        }
    } while (Process32Next(snapshot, &process));

    qDebug() << "Found " << count << " processes.\n";

    CloseHandle(snapshot);

    return count;
}


void CLoxoneApp::killAllRunningApps() {
    QProcess process;
    process.start("taskkill", QStringList() << "/im" << "Loxone.exe" << "/f");
    process.waitForFinished();
}
