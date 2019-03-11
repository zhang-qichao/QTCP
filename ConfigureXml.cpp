#include <QFile>
#include <QDomDocument>
#include <QDomProcessingInstruction>
#include <QDomElement>
#include <QDomNodeList>
#include <QDomNode>
#include <QTextStream>
#include <QDebug>

#include "ConfigureXml.h"
#define cout qDebug() << "[" <<__FILE__<< ":" <<__LINE__<<"]"

/**
 * @brief ConfigureXml::ConfigureXml
 */
ConfigureXml::ConfigureXml()
{
    m_csIP = "";
    m_iPort = 0;
    m_hostName = "";
    m_i_datePort = 0;
    m_dateName = "";
    m_username = "";
    m_password = "";

}

/**
 * @brief ConfigureXml::createSeverXml
 * @param filepath
 */
bool ConfigureXml::createSeverXml(QString filepath)
{
    QFile file(filepath);
    if(file.exists())
    {
        cout<<"file exist";
        return true;
    }
    else
    {
        bool isOk = file.open(QIODevice::WriteOnly);
        if(isOk)
        {
            QDomDocument doc;

            QDomProcessingInstruction ins;
            ins = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\" ");
            doc.appendChild(ins);

            QDomElement root = doc.createElement("Server");
            doc.appendChild(root);

            QDomElement ipEmt = doc.createElement(QString::fromLocal8Bit("ip"));
            QDomText text = doc.createTextNode("127.0.0.1");
            ipEmt.appendChild(text);
            root.appendChild(ipEmt);

            QDomElement portEmt = doc.createElement(QString::fromLocal8Bit("port"));
            text = doc.createTextNode("8888");
            portEmt.appendChild(text);
            root.appendChild(portEmt);

            QTextStream stream(&file);
            doc.save(stream, 4);

            file.close();
        }
        else
        {
            cout<<"WriteOnly error";
        }
    }
}

/**
 * @brief ConfigureXml::createDatebaseXml
 * @param filepath
 * @return
 */
bool ConfigureXml::createDatebaseXml(QString filepath)
{
    QFile file(filepath);
    if(file.exists())
    {
        cout<<"file exist";
        return true;
    }
    else
    {
        if(file.open(QIODevice::WriteOnly))
        {
            QDomDocument doc;

            QDomProcessingInstruction ins;
            ins = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\" ");
            doc.appendChild(ins);

            QDomElement sqlRoot = doc.createElement("Datebase");
            doc.appendChild(sqlRoot);

            QDomElement sqlHostName= doc.createElement(QString::fromLocal8Bit("Hostname"));
            QDomText text = doc.createTextNode("localhost");
            sqlHostName.appendChild(text);
            sqlRoot.appendChild(sqlHostName);

            QDomElement sqlPort= doc.createElement(QString::fromLocal8Bit("port"));
            text = doc.createTextNode("3306");
            sqlPort.appendChild(text);
            sqlRoot.appendChild(sqlPort);

            QDomElement sqlDatename= doc.createElement(QString::fromLocal8Bit("datename"));
            text = doc.createTextNode("Student");
            sqlDatename.appendChild(text);
            sqlRoot.appendChild(sqlDatename);

            QDomElement sqlUsername= doc.createElement(QString::fromLocal8Bit("username"));
            text = doc.createTextNode("root");
            sqlUsername.appendChild(text);
            sqlRoot.appendChild(sqlUsername);

            QDomElement sqlPassword= doc.createElement(QString::fromLocal8Bit("password"));
            text = doc.createTextNode("z314");
            sqlPassword.appendChild(text);
            sqlRoot.appendChild(sqlPassword);


            QTextStream stream(&file);
            doc.save(stream, 4);

            file.close();
        }
        else
        {
            cout<<"WriteOnly error";
        }
    }
}


/**
 * @brief ConfigureXml::readSeverXml
 * @param path
 */
bool ConfigureXml::readSeverXml(QString path)
{
    QFile file(path);
    if(!file.open(QFile::ReadOnly))
        return false;

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return false;
    }
    file.close();

    QDomElement root=doc.documentElement();
    qDebug()<<root.nodeName();

    QDomNodeList list=root.childNodes();
    for(int i=0;i<list.size();i++)
    {
        QDomNode t_n=list.at(i);
        if (t_n.nodeName() == "ip")
        {
            m_csIP = t_n.toElement().text();
        }
        if (t_n.nodeName() == "port")
        {
            m_iPort = t_n.toElement().text().toInt();
        }
    }
            return true;
}

/**
 * @brief ConfigureXml::readDatebaseXml
 * @param path
 * @return
 */
bool ConfigureXml::readDatebaseXml(QString path)
{
    qDebug()<<"root.nodeName()";
    QFile file(path);
    if(!file.open(QFile::ReadOnly))
        return false;

    qDebug()<<"root.nodeName()";
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return false;
    }
    file.close();

    qDebug()<<"root.nodeName()";
    QDomElement root=doc.documentElement();
    qDebug()<<root.nodeName();
    qDebug()<<"root.nodeName()";

    QDomNodeList list=root.childNodes();
    for(int i=0;i<list.size();i++)
    {
        QDomNode t_n=list.at(i);
        if (t_n.nodeName() == "Hostname")
        {
            m_hostName = t_n.toElement().text();
        }
        if (t_n.nodeName() == "port")
        {
            m_i_datePort = t_n.toElement().text().toInt();
        }
        if(t_n.nodeName() == "datename")
        {
            m_dateName = t_n.toElement().text();
        }
        if(t_n.nodeName() == "username")
        {
            m_username = t_n.toElement().text();
        }
        if(t_n.nodeName() == "password")
        {
            m_password = t_n.toElement().text();
        }
    }
            return true;
}

/**
 * @brief ConfigureXml::getIP
 * @return
 */
QString ConfigureXml::getIP()
{
    return m_csIP;
}

/**
 * @brief ConfigureXml::getPort
 * @return
 */
int ConfigureXml::getPort()
{
    return m_iPort;
}

/**
 * @brief ConfigureXml::getHostname
 * @return
 */
QString ConfigureXml::getHostname()
{
    return m_hostName;
}

/**
 * @brief ConfigureXml::getDatebasePort
 * @return
 */
int ConfigureXml::getDatebasePort()
{
    return m_i_datePort;
}

/**
 * @brief ConfigureXml::getDatename
 * @return
 */
QString ConfigureXml::getDatename()
{
    return m_dateName;
}
/**
 * @brief ConfigureXml::getUsername
 * @return
 */
QString ConfigureXml::getUsername()
{
    return m_username;
}

/**
 * @brief ConfigureXml::getPassword
 * @return
 */
QString ConfigureXml::getPassword()
{
    return m_password;
}


