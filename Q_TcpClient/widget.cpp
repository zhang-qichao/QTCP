#include <QFile>
#include <QDomDocument>
#include <QApplication>
#include <QNetworkConfigurationManager>

#include "widget.h"
#include "ui_widget.h"
#include "ConfigureXml.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    tcpSocket = new QTcpSocket(this);
    setWindowTitle("Client1");

    connect(tcpSocket, &QTcpSocket::connected,this,&Widget::dealConnect);
    connect(tcpSocket, &QTcpSocket::readyRead,this,&Widget::readInfo);

    ConfigureXml domXML;
    QString str = QApplication::applicationDirPath()+"/tcp.xml";
    domXML.createXML(str);
    domXML.readXML(str);
    ui->lineEditIP->setText(domXML.getIP());
    ui->lineEditPort->setText(QString::number(domXML.getPort()));

}

Widget::~Widget()
{
    delete ui;
}


/**
 * @brief Widget::dealConnect
 */
void Widget::dealConnect()
{
    ui->textEditRead->setText("connect success");
}

/**
 * @brief Widget::readInfo
 */
void Widget::readInfo()
{
    QString str = tcpSocket->readAll();
    ui->textEditRead->append(str);
}

/**
 * @brief Widget::on_btnConnect_clicked
 */
void Widget::on_btnConnect_clicked()
{

    QString flag = ui->btnConnect->text();
    if(flag == "connect")
    {
    QString ip = ui->lineEditIP->text();
    qint16 port = ui->lineEditPort->text().toInt();
    tcpSocket->connectToHost(ip,port);

    ui->btnConnect->setText("disconnect");
    }
    else
    {
        tcpSocket->disconnectFromHost();
        tcpSocket->close();

        ui->btnConnect->setText("connect");
        ui->textEditRead->setText("disconnect");
    }
}

/**
 * @brief Widget::on_btnSend_clicked
 */
void Widget::on_btnSend_clicked()
{
    if(NULL == tcpSocket)
    {
        return;
    }
    StudentOrEngineerInfo t_p;
    t_p.type = ui->comboBoxChoice->currentIndex();
    memcpy(&t_p.name, ui->lineEditName->text().toUtf8().data(), 12);
    t_p.age=ui->lineEditAge->text().toInt();
    if(ui->sex_comboBox->currentIndex())
    {
        t_p.sex = true;
    }
    else
    {
        t_p.sex = false;
    }
    t_p.scoreOrWorkage=ui->lineEditScore->text().toFloat();

    tcpSocket->write((const char*)&t_p, sizeof(StudentOrEngineerInfo));
//    ui->lineEditName->clear();
//    ui->lineEditAge->clear();
//    ui->lineEditScore->clear();
}

/**
 * @brief Widget::on_comboBoxChoice_activated
 * @param index
 */
void Widget::on_comboBoxChoice_activated(int index)
{
     index=ui->comboBoxChoice->currentIndex();
     if(index == 1)
     {
         ui->labelScore->setText(QString::fromLocal8Bit("工龄"));
     }
     else
     {
         ui->labelScore->setText(QString::fromLocal8Bit("分数"));
     }

}

/**
 * @brief Widget::on_btnClose_clicked
 */
void Widget::on_btnClose_clicked()
{
    this->hide();
}
