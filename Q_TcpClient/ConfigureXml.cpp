#include "ConfigureXml.h"
#include <QFile>
#include <QDomDocument>
#include <QDomProcessingInstruction>
#include <QDomElement>
#include <QTextStream>
#include <QApplication>
#include <QDebug>
#define cout qDebug() << "[" <<__FILE__<< ":" <<__LINE__<<"]"

ConfigureXml::ConfigureXml()
{
  m_csIP = "";
  m_iPort = 0;
}

void ConfigureXml::createXML(QString filepath)
{
    QFile file(filepath);
    if(true == file.exists())
    {
        cout<<"file exist";
        return;
    }
    else
    {
        bool isOk = file.open(QIODevice::WriteOnly);
        if(true == isOk)
        {
            QDomDocument doc;
            QDomProcessingInstruction ins;
            ins = doc.createProcessingInstruction("xml", "version=\'1.0\' encoding=\'utf-8\'");

            doc.appendChild(ins);

            QDomElement root = doc.createElement(QString::fromLocal8Bit("服务器"));
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
 * @brief ConfigureXml::readXML
 * @param path
 */
void ConfigureXml::readXML(QString path)
{
    QFile file(path);
    if(!file.open(QFile::ReadOnly))
        return;

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
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



