#include "selection.h"
#include "ui_selection.h"
#include "widget.h"

#include <QPushButton>
#include <QTabWidget>

SELECTION::SELECTION(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SELECTION)
{
    ui->setupUi(this);

    connect(ui->btnRealTime,SIGNAL(clicked(bool)),this,SLOT(toRealtime(bool)));
    connect(ui->btnHistory,SIGNAL(clicked(bool)),this,SLOT(toHistory(bool)));

}

SELECTION::~SELECTION()
{
    delete ui;
}

void SELECTION::toRealtime(bool)
{
    done(QDialog::Accepted);
    this->close();
}

void SELECTION::toHistory(bool)
{
    done(QDialog::Rejected);
    this->close();
}
