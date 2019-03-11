#ifndef SELECTION_H
#define SELECTION_H

#include <QDialog>
#include "widget.h"


namespace Ui {
class SELECTION;
}

class SELECTION : public QDialog
{
    Q_OBJECT

public:
    explicit SELECTION(QWidget *parent = 0);
    ~SELECTION();

private slots:
    void toRealtime(bool);
    void toHistory(bool);

private:
    Ui::SELECTION *ui;
};

#endif // SELECTION_H
