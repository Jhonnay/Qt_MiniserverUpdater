#include "CConfigMSUpdate.h"
#include "CMiniserver.h"


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

#define LOADING_CYCLE_START_CONFIG  0
#define LOADING_CYCLE_SEND_CONNECT 1
#define LOADING_CYCLE_CHECK_CONNECTING 2
#define LOADING_CYCLE_CHECK_CONNECTED 3
#define LOADING_CYCLE_SEND_LOAD_COMMAND 4
#define LOADING_CYCLE_CHECK_PROGRAM_IDENTICAL 5 



void CConfigMSUpdate::OpenConfigLoadProject(QThread* thread)
{
    int loadingCycle = 0;
    int errorCount = 0;
    CUDPListener udpListener = CUDPListener(nullptr, projectPath, configPath);
    QString connectCommand = QStringLiteral("C,%1,%2,%3").arg(msIP, user, pw).replace(":", ".");

    while (loadingCycle <= LOADING_CYCLE_CHECK_PROGRAM_IDENTICAL && !thread->isInterruptionRequested()) {
        switch (loadingCycle) {
        case LOADING_CYCLE_START_CONFIG:
            errorCount = 0; 
            qDebug() << "---------------------------- Starting Config: Config Path = " << configPath << ", Language" << CConfig::LanguageList.at(configLanguage.toInt()) << ", Projectpath: " << projectPath << "----------------------------";

            config.startConfig_Language(configPath, projectPath, configLanguage);
            udpListener.start();

            loadingCycle++;
            break;


        case LOADING_CYCLE_SEND_CONNECT:
            qDebug() << "---------------------------- Connecting to MS with:" << msIP << "," << user << "," << pw << "----------------------------";
            config.sendCommand(QStringLiteral("127.0.0.1"), 7770, connectCommand); // Connect with MS
            loadingCycle++;
            break;
        
        case LOADING_CYCLE_CHECK_CONNECTING:
            if (udpListener.m_autoStatus.m_nCurrentState <  AUTO_CONNECTING) {
                errorCount++;
                
            }

            if (udpListener.m_autoStatus.m_nCurrentState == AUTO_CONNECTED) {
                errorCount = 0;
                loadingCycle++;

            }

            if (errorCount > 2) { //5 seconds
                loadingCycle--;
                errorCount = 0;
            }
            break;

        case LOADING_CYCLE_CHECK_CONNECTED:
            if (udpListener.m_autoStatus.nDocState == DS_ONLINE) {
                qDebug() << "---------------------------- Miniservers are connected and running. Going to next step - Loading From MS ----------------------------";
                PrintConfigMsVersions(udpListener);
                loadingCycle++;
                break;
            }
            else
            {
                errorCount++;
                qDebug() << "---------------------------- Connection not established or not all Miniservers are running.----------------------------";
                qDebug() << "DocState = " << udpListener.m_autoStatus.nDocState << " should be  =  " << DS_ONLINE;
                
                if (errorCount > 10) {
                    loadingCycle = LOADING_CYCLE_SEND_CONNECT;
                }
            }
            break; 

        case LOADING_CYCLE_SEND_LOAD_COMMAND:
            qDebug() << "---------------------------- Loading from Miniserver ----------------------------";
            errorCount = 0;
            config.LoadFromMiniserver();
            loadingCycle++;
            break;


        case LOADING_CYCLE_CHECK_PROGRAM_IDENTICAL:
            if (udpListener.m_autoStatus.nDocState == DS_ONLINE && udpListener.m_autoStatus.m_nCurrentState == AUTO_CONNECTED && udpListener.m_autoStatus.bAllClientsRunning == 1
                && udpListener.m_autoStatus.bIdentical == 1) {
                qDebug() << "----------------------------- Project successfully loaded from the MS. Next Step - Hurrrrraaaaayyyyyyyy ----------------------------";
                PrintConfigMsVersions(udpListener);
                loadingCycle++;
            }
            else
            {
                qDebug() << "----------------------------- Project not loaded successfully. ----------------------------- ";
                errorCount++;
                if (errorCount > 10) {
                    loadingCycle--;
                }
            }
            break;

        }
        qDebug() << "errorCount= " << errorCount + ", cycle= " << loadingCycle;
        QThread::msleep(1500);
    }
    

    qDebug() << "Closing udp listener";
    
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

void CConfigMSUpdate::buildminiserversVersionString(CUDPListener& udpL)
{
    try
    {
        for (const auto& vers : udpL.m_versionsOfMiniservers)
        {
            miniserverVersions+=  CMiniserver::formatMiniserverVersionQString(vers) +  ", ";
        }
        miniserverVersions = miniserverVersions.left(miniserverVersions.lastIndexOf(","));
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


int CConfigMSUpdate::performMiniserverUpdate(QThread* thread) {

    int updateCycleState = 0;
    int counterCyclesTime = 0; // After 17 Minutes, stop update Progress in this application and show MessageBox and DO NOT KILL CONFIG!
    int ret = 0;
    int fails = 0;
    bool oneMiniserverDidNotUpdate = false;

    CUDPListener udpL = CUDPListener(nullptr, projectPath, configPath);
    udpL.start();
    
    while (udpL.m_autoStatus.nDocState ) {
        QThread::msleep(500); //wait until first data is received
    }

    if (udpL.m_autoStatus.nDocState == DS_ONLINE &&
        udpL.m_autoStatus.m_nCurrentState == AUTO_CONNECTED &&
        udpL.m_autoStatus.bAllClientsRunning == 1) {
        updateCycleState = UPDATE_CYCLE_MS_UPDATE;
        /* this function was coppied from WPF Project.Loading not necessary because we already load from the miniserver with
         OpenConfigLoadProject()   */
    }

    while (updateCycleState < 6 && !thread->isInterruptionRequested())
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
                QThread::msleep(3000);
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
                QThread::msleep(3000);
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
                    //QMessageBox::warning(nullptr, "Error", "One Miniserver did not update! Update will be aborted!");
                    ret = 0;
                }
            }

            if (!oneMiniserverDidNotUpdate)
            {
                qDebug() << "\n" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
                qDebug() << "Versions of Config and Miniservers match";
                PrintConfigMsVersions(udpL);
                updateCycleState++;
                miniserverVersionAfterUpdate = udpL.m_versionsOfMiniservers[0];
                buildminiserversVersionString(udpL);
                ret = 1;
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