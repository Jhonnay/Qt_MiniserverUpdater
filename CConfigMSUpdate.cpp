#include "CConfigMSUpdate.h"


//#define AUTO_DISCONNECTED 0
//#define AUTO_IDLE 1
//#define AUTO_CONNECTING 2
//#define AUTO_CONNECTED 3
//#define AUTO_LOADING 10
//#define AUTO_LOADINGCOMPLETE 11
//#define AUTO_SAVING 20
//#define AUTO_SAVINGCOMPLETE 21
//#define AUTO_UPDATING 30
//#define AUTO_UPDATINGCOMPLETE 31
//#define AUTO_NETSEARCH 40
//#define AUTO_LNKSEARCH 41
//#define AUTO_AIRSEARCH 42
//#define AUTO_ENDSEARCH 49
//
//#define DS_OFFLINE 0
//#define DS_ONLINE 1
//#define DS_SIMULATION 2
//#define DS_LIVEVIEW 3
//
//#define UPDATE_CYCLE_CHECK_CONNECTION_AND_LOAD_PROJECT 0
//#define UPDATE_CYCLE_MS_UPDATE 1
//#define UPDATE_CYCLE_CHECK_UPDATE 2
//#define UPDATE_CYCLE_CHECK_MS_VERSIONS 3
//#define UPDATE_CYCLE_DISCONNECT_CONFIG 4
//#define UPDATE_CYCLE_CLOSE_CONFIG 5


void CConfigMSUpdate::OpenConfigLoadProject()
{
    int errorCount = 0;
    qDebug() << "Starting Config: Config Path = " << configPath << ", Language" << CConfig::LanguageList.at(configLanguage.toInt())
        << ", Projectpath: " << projectPath;

    config.startConfig_Language(configPath, projectPath, configLanguage);
    QThread::sleep(5);
    
    CUDPListener udpListener = CUDPListener(nullptr,projectPath,configPath);
    udpListener.start();

    QString msg = QStringLiteral("C,%1,%2,%3").arg(msIP, user, pw);
    msg = msg.replace(":", ".");
    qDebug() << "Connecting to MS with:" << msIP << "," << user << "," << pw;

 connect:
    config.sendCommand(QStringLiteral("127.0.0.1"), 7770, msg); // Connect with MS

checkConnected:
    qDebug() << "AutoStatus: " << udpListener.m_autoStatus.toString();
    if (udpListener.m_autoStatus.nDocState == DS_ONLINE) {
        qDebug() << "Miniservers are connected and running. Going to next step - Loading From MS";
        PrintConfigMsVersions(udpListener);
        errorCount = 0;
    }
    else
    {
        errorCount++;
        qDebug() << "Connection not established or not all Miniservers are running.";
        qDebug() << "DocState = " << udpListener.m_autoStatus.nDocState << " should be  =  " << DS_ONLINE;
        qDebug() << "Sleep 10s";
        QThread::sleep(10);
        if (errorCount < 5) {
            goto checkConnected;
        }
        else {
            errorCount = 0;
            goto connect;
        }
        
    }
    config.LoadFromMiniserver();
    qDebug() << "ErrorCoung = " << errorCount;
    QThread::sleep(10);
    errorCount = 0;
    qDebug() << "ErrorCoung = " << errorCount;

loadProject:

    if (udpListener.m_autoStatus.nDocState == DS_ONLINE && udpListener.m_autoStatus.m_nCurrentState == AUTO_CONNECTED && udpListener.m_autoStatus.bAllClientsRunning == 1
        && udpListener.m_autoStatus.bIdentical == 1) {
        qDebug() << "Project successfully loaded from the MS. Next Step - Hurrrrraaaaayyyyyyyy";
        PrintConfigMsVersions(udpListener);
    }
    else
    {

        qDebug() << "Project not loaded successfully. Trying again after Sleep 10s";
        qDebug() << "Error Coung = " << errorCount;
        QThread::sleep(10);
        errorCount++;
        if (errorCount < 5) {
            goto loadProject;
        }
        else {
            goto checkConnected;
            errorCount = 0;
        }
        
    }

    udpListener.requestInterruption();
    udpListener.wait();
}


void CConfigMSUpdate::PrintConfigMsVersions(CUDPListener& udpL)
{
    qDebug() << "Current Config Version: " << udpL.m_versionConfig;
    qDebug() << "Current MS-Firmware-Versions: ";

    try
    {
        for (const auto& vers : udpL.m_versionsOfMiniservers)
        {
            qDebug() << vers << ", ";
        }
    }
    catch (const std::exception& ex)
    {
        qDebug() << "SocketException: " << ex.what();
    }
}

int CConfigMSUpdate::getRunningConfigInstances()
{
    const std::wstring target1 = L"LoxoneConfig";
    const std::wstring target2 = L"LoxoneConfig.exe";
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
        if (process.szExeFile == target1 || process.szExeFile == target2) {
            count++;
        }
    } while (Process32Next(snapshot, &process));

    qDebug() << "Found " << count << " processes.\n";

    CloseHandle(snapshot);

    return count;
}


int CConfigMSUpdate::performMiniserverUpdate() {

    int updateCycleState = 0;
    int counterCyclesTime = 0; // After 17 Minutes, stop update Progress in this application and show MessageBox and DO NOT KILL CONFIG!
    int ret = -1;
    int fails = 0;
    bool oneMiniserverDidNotUpdate = false;

    CUDPListener udpL = CUDPListener(nullptr, projectPath, configPath);
    udpL.start();

    if (udpL.m_autoStatus.nDocState == DS_ONLINE &&
        udpL.m_autoStatus.m_nCurrentState == AUTO_CONNECTED &&
        udpL.m_autoStatus.bAllClientsRunning == 1) {
        updateCycleState = UPDATE_CYCLE_MS_UPDATE;
        /* this function was coppied from WPF Project.Loading not necessary because we already load from the miniserver with
         OpenConfigLoadProject()   */
    }

    while (updateCycleState < 6)
    {
        qDebug() << udpL.m_autoStatus.toString();
        switch (updateCycleState)
        {
        case UPDATE_CYCLE_CHECK_CONNECTION_AND_LOAD_PROJECT:
            qDebug() << "Checking if Connection with MS was established and all Clients are also Running.";
            if (udpL.m_autoStatus.nDocState == DS_ONLINE)
            {
                

                qDebug() << "Miniservers are connected and running. Going to next step - Loading From MS. Waiting 1000ms ...";
                QThread::msleep(3000);
                PrintConfigMsVersions(udpL);
                config.LoadFromMiniserver();
                QThread::msleep(7000);
                updateCycleState++;
            }
            else
            {
                qDebug() << "Connection not established or not all Miniservers are running.";
                qDebug() << "DocState = " << udpL.m_autoStatus.nDocState << " should be = " << DS_ONLINE;
                qDebug() << "Sleep 5s";

                fails++;
                if (fails == 5)
                {
                    qDebug() << "Loading from Miniserver again after 5 fails";
                    fails = 0;
                    QString msg = "C," + msIP + "," + user + "," + pw;
                    msg = msg.replace(":", ".");
                    qDebug() << "Connecting to MS with: " << msIP << "," << user << "," << pw;
                    config.sendCommand(localhost, 7770, msg); // Connect with MS
                }
                QThread::msleep(5000);
            }
            break;

        case UPDATE_CYCLE_MS_UPDATE:
            if (udpL.m_autoStatus.nDocState == DS_ONLINE &&
                udpL.m_autoStatus.m_nCurrentState == AUTO_CONNECTED &&
                udpL.m_autoStatus.bAllClientsRunning == 1)
            {
                qDebug() << "Project successfully loaded from the MS. Next Step update Firmware on all MS.";
                config.Update();
                updateCycleState++;
                QThread::msleep(10000); // extra time until the update flag is set 

                if (udpL.m_autoStatus.m_bUpdating != 1)
                {
                    qDebug() << "Miniserver not updating after 10s. Update command will be sent again!";
                    updateCycleState--;
                }
            }
            else
            {
                updateCycleState--;
            }
            break;

        case UPDATE_CYCLE_CHECK_UPDATE:
            if (udpL.m_autoStatus.nDocState == 1 &&
                udpL.m_autoStatus.bAllClientsRunning == 1 &&
                udpL.m_autoStatus.m_bUpdating == 0 &&
                udpL.m_autoStatus.m_nCurrentState == 3)
            {
                qDebug() << "Update Complete. Next Step checking Versions of all Miniservers.";
                updateCycleState++;
            }
            else
            {
                qDebug() << "Miniservers are still updating or restarting.";
            }
            break;


        case UPDATE_CYCLE_CHECK_MS_VERSIONS:
           foreach(QString version, udpL.m_versionsOfMiniservers)
            {
                if (udpL.m_versionConfig != version)
                {
                    oneMiniserverDidNotUpdate = true;
                }
            }

            if (!oneMiniserverDidNotUpdate)
            {
                qDebug() << "\n" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
                qDebug() << "Versions of Config and Miniservers match";
                PrintConfigMsVersions(udpL);
                updateCycleState++;
            }
            break;

        case UPDATE_CYCLE_DISCONNECT_CONFIG:
            // Disconnecting
            config.Disconnect();

            // Check if disconnected!
            qDebug() << "Disconnecting Config from Miniserver";
            updateCycleState++;
            break;

        case UPDATE_CYCLE_CLOSE_CONFIG:
            config.closeConfig();
            qDebug() << "\n\n" << "Update Process complete.";
            updateCycleState++;
            ret = 1;
            break;
        }
        qDebug() << "Waiting 1000ms ...";
        counterCyclesTime += 1;
        if (counterCyclesTime > (60 * 17))
        {
            updateCycleState = 99; // Set > 6 to escape Update While LOOP.
        }
        QThread::msleep(1000);
    }

    udpL.requestInterruption();
    udpL.wait();

    return ret;

}