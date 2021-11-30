#include "patternoperation.h"
#include "ui_patternoperation.h"
#include <QMessageBox>
#include <QDesktopWidget>

PatternOperation::PatternOperation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatternOperation)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width()-this->width())/2,(deskdop->height()-this->height())/2);
    progressPreTime=0;
    onlyFirstTimeUsing=0;
    onlyFirstTimeUsingUCO=0;
}

PatternOperation::~PatternOperation()
{
    delete ui;
}

void PatternOperation::getProgressData(int totalBytes,int currentBytes,const QString &filename)
{
    ui->label_FileName->setText(filename);
    onlyFirstTimeUsing++;
    if(onlyFirstTimeUsing==1)
    {
         onlyFirstTimeUsing=0;
         ui->progressBar->setRange(0,totalBytes-1);
    }
    if(totalBytes>currentBytes)
    {
        //progressPreTime+=currentBytes;
        //ui->progressBar->setValue(progressPreTime);
        ui->progressBar->setValue(currentBytes);
       // ui->label->setText(QString::number(progressPreTime));
    }
    else
    {
       onlyFirstTimeUsing=0;
       //progressPreTime=0;
       close();
    }

}
void PatternOperation::update_slot()
{

}
QProgressBar *PatternOperation::proBar()
{
    return ui->progressBar;
}
