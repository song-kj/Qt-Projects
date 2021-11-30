#include "neworder.h"
#include "ui_neworder.h"
#include "myheader.h"

extern bool global_bRemoteSetting;
extern qint16 totalStep_wifi;
extern qint16 currentStep_wifi;//当前步数
extern bool global_bFromServer;
bool global_bResetSumOutp;//追加产量标志位

NewOrder::NewOrder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewOrder)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    step_Timer = new QTimer(this);
    connect(step_Timer,SIGNAL(timeout()),this,SLOT(step_Slot()));
    m_Info = new ErrorCode;
    global_bResetSumOutp = false;
}

NewOrder::~NewOrder()
{
    delete ui;
}

void NewOrder::ShowInfo(const QString &str)
{
    ui->label->setText(str);
    this->show();
}

//点击是
void NewOrder::on_pushButton_OK_clicked()
{
    if(Ram.Receive(0x100)==0x20)//编织界面且正在编织
    {
        //发单停指令
        emit single_stop_signal();
        m_Info->DisErrorMsg(QMessageBox::Information,tr("注意，当前件织完时会自动停车！"));
        step_Timer->start(20);
    }
    else
    {
        if(step_Timer->isActive())
            step_Timer->stop();
    }
    this->close();
}

//点击否
void NewOrder::on_pushButton_Cancel_clicked()
{
    if(step_Timer->isActive())
        step_Timer->stop();
    this->close();
}

void NewOrder::step_Slot()
{
    if(realtime_parameter.curstep==realtime_parameter.totalstep)    {
        step_Timer->stop();
        m_Info->DisErrorMsg(QMessageBox::Information,tr("请在文件操作菜单中下载花型！"));
    }
}

