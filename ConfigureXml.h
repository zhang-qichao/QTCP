#ifndef ConfigureXml_H
#define ConfigureXml_H

#include <QString>

/**
 * @brief The ConfigureXml class
 */
class ConfigureXml
{
public:
    ConfigureXml();

    bool createSeverXml(QString filepath);
    bool readSeverXml(QString);

    bool createDatebaseXml(QString filepath);
    bool readDatebaseXml(QString);

    QString getIP();
    int getPort();
    QString getHostname();
    int getDatebasePort();
    QString getDatename();
    QString getUsername();
    QString getPassword();

private:
    QString m_csIP;
    int m_iPort;
    QString m_hostName;
    int m_i_datePort;
    QString m_dateName;
    QString m_username;
    QString m_password;
};

#endif // ConfigureXml_H
