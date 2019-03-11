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

    void createXML(QString filepath);
    void readXML(QString);

    QString getIP();
    int getPort();

private:
    QString m_csIP;
    int m_iPort;
};

#endif // ConfigureXml_H
