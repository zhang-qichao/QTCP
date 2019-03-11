#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>

#include "base.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void dealConnect();
    void readInfo();

private slots:
    void on_btnConnect_clicked();

    void on_btnSend_clicked();

    void on_comboBoxChoice_activated(int index);

    void on_btnClose_clicked();

private:
    Ui::Widget *ui;

    /**
     * @brief tcpSocket
     */
    QTcpSocket *tcpSocket;
    QList<StudentOrEngineerInfo> m_StudentList;
};

#endif // WIDGET_H
