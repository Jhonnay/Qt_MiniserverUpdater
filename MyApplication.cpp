#include "MyApplication.h"
#include <qmessagebox.h>
#include <QString>
#include <stdexcept>



bool MyApplication::notify(QObject* receiver, QEvent* event) {
    __try {
        return QApplication::notify(receiver, event);
    }
    __except(ExceptionHandler(GetExceptionInformation())) {
        // Handle the exception here, for example, show an error message.
        // You can also log the exception details for debugging purposes.
        
        //qCritical() << "Exception caught:" << e.what();
        //QMessageBox::warning(nullptr, "Error", e.what());
        //ExceptionHandler(((struct _EXCEPTION_POINTERS*)_exception_info()));
        return false; // Prevent the application from crashing
    }
}


LONG WINAPI MyApplication::ExceptionHandler(EXCEPTION_POINTERS* pExceptionInfo) {
    QString exceptionMessage;
    
    DWORD exceptionCode = pExceptionInfo->ExceptionRecord->ExceptionCode;

    if (exceptionCode == EXCEPTION_ACCESS_VIOLATION) {
        exceptionMessage = "Access violation exception occurred.";
    }
    else if (exceptionCode == EXCEPTION_STACK_OVERFLOW) {
        exceptionMessage = "Stack overflow exception occurred.";
    }
    else {
        exceptionMessage = "An unknown exception occurred.";
    }
    
    QMessageBox::critical(nullptr, "Something happened", exceptionMessage);

    int result = MessageBox(nullptr, (LPCSTR)"An exception occurred. Do you want to save a crash dump?", (LPCSTR)"Crash Dump", MB_YESNO | MB_ICONERROR);

    if (result == IDYES) {
        OPENFILENAME ofn;
        wchar_t szFile[MAX_PATH] = L"";

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.lpstrFile = (LPSTR)szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = (LPCSTR)"Dump Files (*.dmp)\0*.dmp\0All Files (*.*)\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

        if (GetSaveFileName(&ofn)) {
            HANDLE hFile = CreateFile((LPCSTR)szFile, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (hFile != INVALID_HANDLE_VALUE) {
                MINIDUMP_EXCEPTION_INFORMATION eInfo;
                eInfo.ThreadId = GetCurrentThreadId();
                eInfo.ExceptionPointers = pExceptionInfo;
                eInfo.ClientPointers = FALSE;
                MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &eInfo, nullptr, nullptr);
                CloseHandle(hFile);
                MessageBox(nullptr, (LPCSTR)"Crash dump saved successfully.", (LPCSTR)"Crash Dump", MB_OK | MB_ICONINFORMATION);
            }
            else {
                MessageBox(nullptr, (LPCSTR)"Failed to create dump file.", (LPCSTR)"Crash Dump", MB_OK | MB_ICONERROR);
            }
        }
        else {
            MessageBox(nullptr, (LPCSTR)"Dump file not saved.", (LPCSTR)"Crash Dump", MB_OK | MB_ICONINFORMATION);
        }
    }

    return EXCEPTION_EXECUTE_HANDLER;
}
