#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QtSql>
#include <QSqlQuery>
#include "base.h"

namespace Ui {
class Widget;
}

/**
 * @brief The Widget class
 */
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void dealConnect();
    void readInfo();
private slots:
    void on_btnFomer_clicked();

    void on_btnLast_clicked();

    void comboBoxChoice_slot(int);

    void on_btnSure_clicked();

    void on_btnAdd_clicked();

    void on_btnSelect_clicked();

    void tabWidgetCurrentChanged_slot(int);

private:
    Ui::Widget *ui;

    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;

    QSqlDatabase db;
    QSqlQuery query;

    int m_i_receiveStuNum;
    int m_i_receiveEngNum;  //总条数

    int m_i_StuPageCount;
    int m_i_EngPageCount; //页数

    QList<StudentOrEngineerInfo> StudentList;
    QList<StudentOrEngineerInfo> EngineerList;
    /**
     * @brief setStudentInfo
     * @param row
     * @param stu
     */
    void setStudentInfo(int row,StudentOrEngineerInfo stu);
    /**
     * @brief setEngineerInfo
     * @param row
     * @param eng
     */
    void setEngineerInfo(int row,StudentOrEngineerInfo eng);

    /**
     * @brief initSql
     */
    void initSql();

};

#endif // WIDGET_H
