
#include "stdafx.h"
#include "CMiniserver.h"


CMiniserver::CMiniserver()
{
    serialNumber = "empty";
    adminUser = "a";
    adminPassword = "a";
    miniserverStatus="empty";
    updatelevel = "none";
    versionColor = "black";
    miniserverProject = "none";
    miniserverConfiguration = "NAN";
    localIP = "gone";
    configLanguage = "5";

}

CMiniserver::CMiniserver(const std::string& serialNumber, const std::string& adminUser,
    const std::string& adminPassword, const std::string& miniserverVersion,
    const std::string& miniserverStatus, const std::string& updatelevel,
    const std::string& versionColor, const std::string& miniserverProject,
    const std::string& miniserverConfiguration, const std::string& localIP,
    const std::string& configLanguage)
{
    // Set the properties of the CMiniserver object using the values passed in to the constructor
    this->serialNumber = serialNumber;
    this->adminUser = adminUser;
    this->adminPassword = adminPassword;
    this->miniserverVersion = miniserverVersion;
    this->miniserverStatus = miniserverStatus;
    this->updatelevel = updatelevel;
    this->versionColor = versionColor;
    this->miniserverProject = miniserverProject;
    this->miniserverConfiguration = miniserverConfiguration;
    this->localIP = localIP;
    this->configLanguage = configLanguage;

}


// Getter and Setter for serialNumber
std::string CMiniserver::getSerialNumber() const
{
    return serialNumber;
}

void CMiniserver::setSerialNumber(const std::string& serialNumber)
{
    this->serialNumber = serialNumber;
}

// Getter and Setter for adminUser
std::string CMiniserver::getAdminUser() const
{
    return adminUser;
}

void CMiniserver::setAdminUser(const std::string& adminUser)
{
    this->adminUser = adminUser;
}

// Getter and Setter for adminPassword
std::string CMiniserver::getAdminPassword() const
{
    return adminPassword;
}

void CMiniserver::setAdminPassword(const std::string& adminPassword)
{
    this->adminPassword = adminPassword;
}

// Getter and Setter for miniserverVersion
std::string CMiniserver::getMiniserverVersion() const
{
    return miniserverVersion;
}

void CMiniserver::setMiniserverVersion(const std::string& miniserverVersion)
{
    this->miniserverVersion = miniserverVersion;
}

// Getter and Setter for miniserverStatus
std::string CMiniserver::getMiniserverStatus() const
{
    return miniserverStatus;
}

void CMiniserver::setMiniserverStatus(const std::string& miniserverStatus)
{
    this->miniserverStatus = miniserverStatus;
}

// Getter and Setter for updatelevel
std::string CMiniserver::getUpdatelevel() const
{
    return updatelevel;
}

void CMiniserver::setUpdatelevel(const std::string& updatelevel)
{
    this->updatelevel = updatelevel;
}

// Getter and Setter for versionColor
std::string CMiniserver::getVersionColor() const
{
    return versionColor;
}

void CMiniserver::setVersionColor(const std::string& versionColor)
{
    this->versionColor = versionColor;
}

// Getter and Setter for miniserverProject
std::string CMiniserver::getMiniserverProject() const
{
    return miniserverProject;
}

void CMiniserver::setMiniserverProject(const std::string& miniserverProject)
{
    this->miniserverProject = miniserverProject;
}

// Getter and Setter for miniserverConfiguration
std::string CMiniserver::getMiniserverConfiguration() const
{
    return miniserverConfiguration;
}

void CMiniserver::setMiniserverConfiguration(const std::string& miniserverConfiguration)
{
    this->miniserverConfiguration = miniserverConfiguration;
}

// Getter and Setter for localIP
std::string CMiniserver::getLocalIP() const
{
    return localIP;
}

void CMiniserver::setLocalIP(const std::string& localIP)
{
    this->localIP = localIP;
}

std::string CMiniserver::getConfigLanguage() const
{
    return configLanguage;
}

void CMiniserver::setConfigLanguage(const std::string& language)
{
    this->configLanguage = language;
}


QString CMiniserver::formatMiniserverVersionQString(QString unformatedQString) {
    QString formattedQString = "0.0.0.0";

    if (unformatedQString.length() == 8) {
        int unformatedInt = unformatedQString.toInt();
        int v1 = (unformatedInt / 1000000) % 100;
        int v2 = (unformatedInt / 10000) % 100;
        int v3 = (unformatedInt / 100) % 100;
        int v4 = unformatedInt % 100;

        QString part1 = QString::number(v1);
        QString part2 = QString::number(v2);
        QString part3 = QString::number(v3);
        QString part4 = QString::number(v4);

        formattedQString = part1 + "." + part2 + "." + part3 + "." + part4;
    }

    return formattedQString;
}

QString CMiniserver::unformatMiniserverVersionQString(const QString& version) {
    QStringList parts = version.split('.');
    QString convertedVersion;

    for (const QString& part : parts) {
        bool ok;
        int num = part.toInt(&ok);
        if (ok) {
            convertedVersion += QString("%1").arg(num, 2, 10, QChar('0'));
        }
    }

    return convertedVersion;
}


std::string CMiniserver::toString() const
{
    std::string str = "CMiniserver {";
    str += serialNumber + ", ";
    str += adminUser + ", ";
    str += adminPassword + ", ";
    str += miniserverVersion + ", ";
    str += miniserverStatus + ", ";
    str += updatelevel + ", ";
    str += versionColor + ", ";
    str += miniserverProject + ", ";
    str += miniserverConfiguration + ", ";
    str += localIP + " ";
    str += "Lang:" + CConfig::LanguageList.at(stoi(configLanguage)).toStdString() + " }";
    return str;
}

void CMiniserver::printAllMiniserversToDebug(QList<CMiniserver>* miniservers)
{
    for (int i = 0; i < miniservers->count(); ++i)
    {
        std::string message = " Index: " + std::to_string(i) + " - " + (*miniservers)[i].toString() + "\n";
        qDebug() << message;
    }
}