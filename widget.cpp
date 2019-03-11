
#include <QFile>
#include <QDomDocument>
#include <QHostAddress>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>

#include "widget.h"
#include "ui_widget.h"
#include "base.h"
#include "ConfigureXml.h"
#include "selection.h"

#define TABLESHOWROW 10


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowTitle("selection");

    m_i_receiveStuNum=0;
    m_i_receiveEngNum=0;

    m_i_EngPageCount = 1;
    m_i_StuPageCount = 1;

    ui->tableStudent->setRowCount(10);
    ui->tableStudent->setColumnCount(4);
    ui->tableEngineer->setRowCount(10);
    ui->tableEngineer->setColumnCount(4);


    ui->tableStudent->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableStudent->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableEngineer->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableEngineer->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->comboBoxPage->insertItem(0,QString::number(m_i_StuPageCount));
    ui->comboBoxPage->setDuplicatesEnabled(false);

    ui->tableStudent->setStyleSheet("QTableWidget{background-color:rgb(210,255,255)}");
    ui->tableEngineer->setStyleSheet("QTableWidget{background-color:rgb(255,210,255)}");

    connect(ui->tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(tabWidgetCurrentChanged_slot(int)));
    connect(ui->comboBoxPage,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxChoice_slot(int)));

    SELECTION k;
    int res = k.exec();
    //选择实时模式
    if (res == QDialog::Accepted)
    {
        ConfigureXml domXML;
        QString strServer = QApplication::applicationDirPath()+"/tcp.xml";
        if(!domXML.createSeverXml(strServer))
        {
            QMessageBox::warning(this,"warning", "create tcpXML failure");
        }
        if(!domXML.readSeverXml(strServer))
        {
            QMessageBox::warning(this,"warning", "open tcpXML failure");
        }

        ui->lineEditIP->setText(domXML.getIP());
        ui->lineEditPort->setText(QString::number(domXML.getPort()));

        QString strDatebase = QApplication::applicationDirPath()+"/Datebase.xml";
        if (!domXML.createDatebaseXml(strDatebase))
        {
            QMessageBox::warning(this, "warning", "create datebase error");
        }
        if (!domXML.readDatebaseXml(strDatebase))
        {
            QMessageBox::warning(this, "warning", "open datebase error");
        }

        ui->SqlLocalHost->setText(domXML.getHostname());
        ui->SqlPort->setText(QString::number(domXML.getDatebasePort()));
        ui->SqldateName->setText(domXML.getDatename());
        ui->SqluserName->setText(domXML.getUsername());
        ui->SqlPassword->setEchoMode(QLineEdit::Password);
        ui->SqlPassword->setText(domXML.getPassword());

        tcpServer = new QTcpServer(this);
        tcpServer->listen(QHostAddress::Any,8888);

        connect(tcpServer,&QTcpServer::newConnection,this,&Widget::dealConnect);
        connect(tcpSocket, &QTcpSocket::readyRead,this,&Widget::readInfo);
    }
    //选择历史模式
    else
    {
        ui->groupBoxServer->hide();
    }
}


Widget::~Widget()
{
    delete ui;
}

/**
 * @brief Widget::readInfo
 */
void Widget::readInfo()
{
    QByteArray array = tcpSocket->readAll();
    StudentOrEngineerInfo t_s;

    if(array.length()<sizeof(t_s))
    {
        return;
    }
    memcpy(&t_s, array.data(), array.length());
    qDebug()<<t_s.name;
    if(t_s.type == false)
    {
        static int stucount = 0;
        if (++stucount>10)
        {
            m_i_StuPageCount++;
            stucount = 0;
            ui->comboBoxPage->insertItem(m_i_StuPageCount-1,QString::number(m_i_StuPageCount));
        }
        StudentList.append(t_s);
        if (m_i_receiveStuNum++ > 10)
            return;
        setStudentInfo(m_i_receiveStuNum,t_s);
    }
    else
    {
        static int engcount = 0;
        if (++engcount>10)
        {
            m_i_EngPageCount++;
            engcount = 0;
            ui->comboBoxPage->insertItem(m_i_EngPageCount-1,QString::number(m_i_EngPageCount));
        }
        EngineerList.append(t_s);
        if (m_i_receiveEngNum++ > 10)
            return;
        setEngineerInfo(m_i_receiveEngNum,t_s);

    }
}

/**
 * @brief Widget::dealConnect
 */
void Widget::dealConnect()
{
    tcpSocket = tcpServer->nextPendingConnection();

    QString ip = tcpSocket->peerAddress().toString();
    qint16 port = tcpSocket->peerPort();

    QString temp = QString("[%1:%2],connect success").arg(ip).arg(port);

    ui->textEditInfo->setText(temp);
    connect(tcpSocket, &QTcpSocket::readyRead,this,&Widget::readInfo);

}

/**
 * @brief Widget::setStudentInfo
 * @param row
 * @param stu
 */
void Widget::setStudentInfo(int row,StudentOrEngineerInfo stu)
{
    ui->tableStudent->setItem(row-1,0,new QTableWidgetItem(QString::fromUtf8(stu.name)));
    ui->tableStudent->setItem(row-1,1,new QTableWidgetItem(QString::number(stu.age)));
    if(stu.sex)
        ui->tableStudent->setItem(row-1,2,new QTableWidgetItem("0"));
    else
        ui->tableStudent->setItem(row-1,2,new QTableWidgetItem("1"));
    ui->tableStudent->setItem(row-1,3,new QTableWidgetItem(QString::number(stu.scoreOrWorkage)));
}
/**
 * @brief Widget::setEngineerInfo
 * @param row
 * @param eng
 */
void Widget::setEngineerInfo(int row,StudentOrEngineerInfo eng)
{
    ui->tableEngineer->setItem(row-1,0,new QTableWidgetItem(QString::fromUtf8(eng.name)));
    ui->tableEngineer->setItem(row-1,1,new QTableWidgetItem(QString::number(eng.age)));
    if(eng.sex)
        ui->tableEngineer->setItem(row-1,2,new QTableWidgetItem("0"));
    else
        ui->tableEngineer->setItem(row-1,2,new QTableWidgetItem("1"));
    ui->tableEngineer->setItem(row-1,3,new QTableWidgetItem(QString::number(eng.scoreOrWorkage)));
}

//翻页
/**
 * @brief Widget::comboBoxChoice_slot
 * @param index
 */
void Widget::comboBoxChoice_slot(int index)
{
    if (index < 0)
        return;
    int tabindex = ui->tabWidget->currentIndex();
    if(tabindex == 0)
    {
        ui->tableStudent->clearContents();
        int j=1;
        int srclength = index*TABLESHOWROW;//某页起点
        int dstlength = StudentList.count()-index*TABLESHOWROW;//对应页终点
        if (dstlength>10)
            dstlength = 10;
        for(int i=srclength;i<srclength+dstlength;i++)
        {
            setStudentInfo(j++,StudentList.at(i));
        }
    }
    else
    {
        ui->tableEngineer->clearContents();
        int j=1;
        int srclength = index*TABLESHOWROW;
        int dstlength = EngineerList.count()-index*TABLESHOWROW;
        if (dstlength>10)
            dstlength = 10;
        for(int i=srclength;i<srclength+dstlength;i++)
        {
            setEngineerInfo(j++,EngineerList.at(i));
        }
    }
}
/**
 * @brief Widget::tabWidgetCurrentChanged_slot
 * @param index
 */
void Widget::tabWidgetCurrentChanged_slot(int index)
{
    ui->comboBoxPage->clear();
    int num = 0;
    if (index == 0)
        num = m_i_StuPageCount;
    else
        num = m_i_EngPageCount;
    for (int i = 0; i < num; i++)
    {
        ui->comboBoxPage->insertItem(i, QString::number(i+1));
    }
}


//上一页
/**
 * @brief Widget::on_btnFomer_clicked
 */
void Widget::on_btnFomer_clicked()
{
    int tabindex = ui->tabWidget->currentIndex();
    int indexNum = ui->comboBoxPage->currentIndex();
    if(tabindex == 0)
    {
        if(indexNum>0)
        {
            int j=0;
            for(int i=(indexNum-1)*TABLESHOWROW;i<indexNum*TABLESHOWROW;i++)
            {
                StudentOrEngineerInfo t_k = StudentList.at(i);
                setStudentInfo(j++,t_k);
            }
            ui->comboBoxPage->setCurrentIndex(indexNum-1);
        }
    }
    else
    {
        if(indexNum>0)
        {
            int j=0;
            for(int i=(indexNum-1)*TABLESHOWROW;i<indexNum*TABLESHOWROW;i++)
            {
                StudentOrEngineerInfo t_k = EngineerList.at(i);
                setEngineerInfo(j++,t_k);
            }
            ui->comboBoxPage->setCurrentIndex(indexNum-1);
        }
    }
}


//下一页
/**
 * @brief Widget::on_btnLast_clicked
 */
void Widget::on_btnLast_clicked()
{
    int tabindex = ui->tabWidget->currentIndex();
    int indexNum=ui->comboBoxPage->currentText().toInt();
    if(tabindex == 0)
    {
    if(indexNum<m_i_StuPageCount)
    {
        int j =0;
        for(int i =indexNum*TABLESHOWROW;i<(indexNum+1);i++)
        {
            StudentOrEngineerInfo t_k = StudentList.at(i);
            setStudentInfo(j++,t_k);
        }
        ui->comboBoxPage->setCurrentIndex(indexNum);
    }
    }
    else
    {
        if(indexNum<m_i_EngPageCount)
        {
            int j =0;
            for(int i =indexNum*TABLESHOWROW;i<(indexNum+1);i++)
            {
                StudentOrEngineerInfo t_k = EngineerList.at(i);
                setEngineerInfo(j++,t_k);
            }
            ui->comboBoxPage->setCurrentIndex(indexNum);
        }
    }
}

/**
 * @brief Widget::on_btnSure_clicked
 */
void Widget::on_btnSure_clicked()
{
    initSql();
}

//获取某行数据添加至数据库
/**
 * @brief Widget::on_btnAdd_clicked
 */
void Widget::on_btnAdd_clicked()
{

    int tabindex = ui->tabWidget->currentIndex();
    if(tabindex == 0)
    {
        int row = ui->tableStudent->currentIndex().row();
        QString name = ui->tableStudent->item(row,0)->text();
        int age = ui->tableStudent->item(row,1)->text().toInt();
        QString sex = ui->tableStudent->item(row,2)->text();
        float score = ui->tableStudent->item(row,3)->text().toFloat();

        qDebug()<<query.exec(QString("INSERT INTO T_Student(Name,Age,Sex,Score) VALUES('%1',%2,'%3',%4)").arg(name).arg(age).arg(sex).arg(score));
    }
    else
    {
        int row = ui->tableEngineer->currentIndex().row();
        QString name = ui->tableEngineer->item(row,0)->text();
        int age = ui->tableEngineer->item(row,1)->text().toInt();
        QString sex = ui->tableEngineer->item(row,2)->text();
        float workage = ui->tableEngineer->item(row,3)->text().toFloat();

        qDebug()<<query.exec(QString("INSERT INTO T_Engineer(Name,Age,Sex,Workage) VALUES('%1',%2,'%3',%4)").arg(name).arg(age).arg(sex).arg(workage));

    }
}

//历史模式选择查询数据库
/**
 * @brief Widget::on_btnSelect_clicked
 */
void Widget::on_btnSelect_clicked()
{
    int t_index =ui->tabWidget->currentIndex();
    if(t_index == 0)
    {
        query.exec(QString("select * from T_Student"));

        m_i_receiveStuNum = 0;
        m_i_StuPageCount = 1;
        StudentList.clear();

        int pagecount = 0;
        while (query.next())
        {
            StudentOrEngineerInfo t_k;
            QString t_name = query.value(1).toString();
            memcpy(&t_k.name, t_name.toUtf8(), 12);
            t_k.age = query.value(2).toInt();
            if (query.value(3).toString() == "0")
            {
                t_k.sex = true;
            }
            else
            {
                t_k.sex = false;
            }
            t_k.scoreOrWorkage = query.value(4).toFloat();
            StudentList<<t_k;

            if (++pagecount>10)
            {
                m_i_StuPageCount++;
                pagecount = 0;
                ui->comboBoxPage->insertItem(m_i_StuPageCount-1,QString::number(m_i_StuPageCount));
            }

            if (m_i_receiveStuNum++ > 10)
                continue;
            setStudentInfo(m_i_receiveStuNum,t_k);
        }
    }
    else
    {
        query.exec(QString("select * from T_Engineer"));

        m_i_receiveEngNum = 0;
        m_i_EngPageCount = 1;
        EngineerList.clear();

        int pagecount = 0;
        while (query.next())
        {
            StudentOrEngineerInfo t_k;
            QString t_name = query.value(1).toString();
            memcpy(&t_k.name, t_name.toUtf8(), 12);

            t_k.age = query.value(2).toInt();
            if (query.value(3).toString() == "0")
            {
                t_k.sex = true;
            }
            else
            {
                t_k.sex = false;
            }
            t_k.scoreOrWorkage = query.value(4).toFloat();
            EngineerList<<t_k;

            if (++pagecount>10)
            {
                m_i_EngPageCount++;
                pagecount = 0;
                ui->comboBoxPage->insertItem(m_i_EngPageCount-1,QString::number(m_i_EngPageCount));
            }

            if (m_i_receiveEngNum++ > 10)
                continue;
            setEngineerInfo(m_i_receiveEngNum,t_k);
        }
    }
}

/**
 * @brief Widget::initSql
 */
void Widget::initSql()
{
    QString hostName = ui->SqlLocalHost->text();
    int port = ui->SqlPort->text().toInt();
    QString dateName = ui->SqldateName->text();
    QString userName = ui->SqluserName->text();
    QString password = ui->SqlPassword->text();


    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(hostName);
    db.setPort(port);
    db.setDatabaseName(dateName);
    db.setUserName(userName);
    db.setPassword(password);

    if(!db.open())
    {
        ui->textEditInfo->setText("open sql datebase failure");
        return;
    }
    else
    {
        ui->textEditInfo->setText("open sql datebase success");
        ui->btnSure->close();
        query = QSqlQuery(db);
    }
}














