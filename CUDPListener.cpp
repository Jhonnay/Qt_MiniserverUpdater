#include "CUDPListener.h"
#include "CConfig.h"
#include <cstdint>
#include "CConfigMSUpdate.h"

CUDPListener::CUDPListener(QObject *parent, const QString& projectPath, const QString& configPath)
	: QThread(parent)
{
    m_projectPath = projectPath;
    m_configPath = configPath;
    m_socket = new QUdpSocket();
    

    m_autoStatus.nDocState = 0;
    m_autoStatus.bAllClientsRunning = 0;
    m_autoStatus.bIdentical = 0;
    m_autoStatus.nMiniservers = 0;
    m_autoStatus.m_bUpdating = 0;
    m_autoStatus.m_nCurrentState = 0;
    m_versionConfig = CConfig::getConfigFileVersionUnformated(configPath);
}



CUDPListener::~CUDPListener()
{}




void CUDPListener::run()
{
    m_socket->bind(QHostAddress::LocalHost, 7771);  // Port to listen on
    while (!isInterruptionRequested()) {
        quint16 senderPort;
        QHostAddress senderIp;
        QByteArray datagram;
        datagram.resize(m_socket->pendingDatagramSize());
        m_socket->readDatagram(datagram.data(), datagram.size(), &senderIp, &senderPort);

        while (!m_socket->hasPendingDatagrams()) {
            QThread::msleep(1000);
        };

        if (datagram == "") {
            continue;//the first datagramm is always empty resulting in a crash. 
        }
        //qDebug() << "Received datagram: " << datagram;
        // Parse status from received data
        m_autoStatus.nDocState = datagram[0];
        m_autoStatus.bAllClientsRunning = datagram[4];
        m_autoStatus.bIdentical = datagram[8];
        m_autoStatus.nMiniservers = datagram[12];
        m_autoStatus.m_bUpdating = datagram[16];
        m_autoStatus.m_nCurrentState = datagram[20];
        
        if (m_autoStatus.nDocState == 1)
        {
            m_versionsOfMiniservers.clear();
            for (int i = 0; i < m_autoStatus.nMiniservers; i++)
            {
                uint32_t temp = *(reinterpret_cast<const uint32_t*>(&datagram[(24 + 4 * i)]));
                m_versionsOfMiniservers.append(QString::number(temp));
            }
        
            qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << " - " << m_autoStatus.toString().toStdString();
            QString message =  "MS-Version ";
            for (const auto& version : m_versionsOfMiniservers)
            {
                message = message + version + ", ";
            }
            qDebug() << message;
        }
        else
        {
            qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << " - " << m_autoStatus.toString() << ", MS-Version NA";
        
        }
        
        
        
        // Log status
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << m_autoStatus.toString() << "MS Versions" << m_versionsOfMiniservers;
        
        // Get config file version
        qDebug() << "Config Version:" << CConfig::getConfigFileVersionUnformated(m_configPath);

    }     
    
    
    m_socket->close();
    QThread::sleep(1);
    if (m_socket->isOpen()) {
        qDebug() << "Failed to close UDP socket";
    }
    else {
        qDebug() << "Socket closed! :) JUhuuuuu";
    }
    
}