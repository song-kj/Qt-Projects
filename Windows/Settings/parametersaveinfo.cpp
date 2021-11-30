#include "parametersaveinfo.h"
#include "ui_parametersaveinfo.h"

ParameterSaveInfo::ParameterSaveInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParameterSaveInfo)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
}

ParameterSaveInfo::~ParameterSaveInfo()
{
    delete ui;
}

void ParameterSaveInfo::setShowText(QString str)
{
    ui->label->setText(str);
}
