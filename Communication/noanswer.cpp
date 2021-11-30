#include "noanswer.h"
#include "ui_noanswer.h"
#include <QDesktopWidget>

extern int flag;
extern int Noanswer_start;
extern QByteArray temp;

Noanswer::Noanswer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Noanswer)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    timer_noanswer=new QTimer(this);
    connect(timer_noanswer,SIGNAL(timeout()),this,SLOT(update_slot()));
    timer_noanswer->start(100);
}

Noanswer::~Noanswer()
{
    delete ui;
}

void Noanswer::on_sure_pb_clicked()
{
    reject();//隐藏对话框
}
void Noanswer::update_slot()
{
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)34)
        {
            temp[0]=255;
            on_sure_pb_clicked();
        }
    }
}
