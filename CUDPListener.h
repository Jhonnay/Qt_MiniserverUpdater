#pragma once

#include "stdafx.h"
#include <QThread>
#include <QThread>
#include <QUdpSocket> 
#include <QFileInfo>
#include <QDebug>

struct AutoStatus {
    
    //State Buffer is 4 Byte aligned 
    
    //int nDocState; 
    //bool bAllClientsRunning; 
    //bool bIdentical;
    //int nMiniserver;
    //bool m_bUpdating;
    //int m_nCurrentState;
    // add more status fields here
    
    int nDocState;
    /*
        #define DS_OFFLINE 0
        #define DS_ONLINE 1
        #define DS_SIMULATION 2
        #define DS_LIVEVIEW 3
     */
    int bAllClientsRunning; //All clients online 
    int bIdentical; //Programm identisch 
    int nMiniservers; //Anzahl Miniserver
    int m_bUpdating; // boolean
    int m_nCurrentState;
    /*
    #define AUTO_DISCONNECTED 0
    #define AUTO_IDLE 1
    #define AUTO_CONNECTING 2
    #define AUTO_CONNECTED 3
    #define AUTO_LOADING 10
    #define AUTO_LOADINGCOMPLETE 11
    #define AUTO_SAVING 20
    #define AUTO_SAVINGCOMPLETE 21
    #define AUTO_UPDATING 30
    #define AUTO_UPDATINGCOMPLETE 31
    #define AUTO_NETSEARCH 40
    #define AUTO_LNKSEARCH 41
    #define AUTO_AIRSEARCH 42
    #define AUTO_ENDSEARCH 49
    
    */
    
    QString toString() const
    {
        return QString("nDocState = %1, bAllClientsRunning = %2, bIdentical = %3, nMiniservers = %4, m_bUpdating = %5, m_nCurrentState = %6")
            .arg(nDocState)
            .arg(bAllClientsRunning)
            .arg(bIdentical)
            .arg(nMiniservers)
            .arg(m_bUpdating)
            .arg(m_nCurrentState);
    }
};


class CUDPListener  : public QThread
{
	Q_OBJECT

public:
	//CUPDListener(QObject *parent);
    CUDPListener(QObject* parent, const QString& projectPath, const QString& configPath);
    //CUDPListener(const QString& projectPath, const QString& configPath);
	~CUDPListener();

    QUdpSocket* m_socket;
    AutoStatus m_autoStatus;
    QString m_versionConfig;
    QList<QString> m_versionsOfMiniservers;
    QString m_projectPath;
    QString m_configPath;

private:
    void run() override;

};
