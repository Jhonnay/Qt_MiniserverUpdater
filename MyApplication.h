#pragma once
#include <qapplication.h>
#include <windows.h>
#include <DbgHelp.h>
#include <Commdlg.h>

class MyApplication : public QApplication
{
public:
	MyApplication(int& argc, char** argv) : QApplication(argc, argv) {};

    virtual bool notify(QObject* receiver, QEvent* event) override;

	LONG __stdcall ExceptionHandler(EXCEPTION_POINTERS* pExceptionInfo);

};

