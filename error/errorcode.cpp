#include "errorcode.h"

ErrorCode::ErrorCode()
{
    this->setFixedSize(200,80);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

ErrorCode::~ErrorCode()
{

}

void ErrorCode::DisErrorMsg(Icon icon,const QString &msg)
{
    this->setButtonText(QMessageBox::Ok,QString("确定"));
    this->setText(tr("<font size='12' color='red'>")+msg+tr("</font>"));
    this->setIcon(icon);
    this->show();
}


