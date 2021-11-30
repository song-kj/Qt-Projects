#include "inputtesting.h"
#include "ui_inputtesting.h"
#include <unistd.h>

extern QByteArray temp;
InputTesting::InputTesting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputTesting)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);
    initialForm();
//    Ram.SendData[0]=0x15;
//    Ram.SendData[1]=0xA1;
//    Ram.SendData[2]=0x55;
//    Ram.SendData[3]=0x00;
//    Ram.SendData[4]=0x00;
//    Ram.SendData[5]=0x00;
//    Ram.SendData[6]=0x00;
//    Ram.SendData[7]=0x00;
//    Ram.SendData[8]=0x00;
//    Ram.SendData[9]=0x00;
//    Ram.UI_to_RTC();
    Ram.to_PatternWoven_slot();
    usleep(5000);
    Ram.SendData[0]=0x20;
    Ram.SendData[1]=0xA1;
    Ram.SendData[2]=0x0B;
    Ram.SendData[3]=0x55;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    timer_input=new QTimer(this);
    connect(timer_input, SIGNAL(timeout()), this, SLOT(update_input_slot()));
    timer_input->start(100);
}

InputTesting::~InputTesting()
{
    delete ui;
}

void InputTesting::initialForm()
{
    QStringList stringlist;
    stringlist<<tr("1号探针")<<tr("2号探针")<<tr("3号探针")<<tr("4号探针")<<tr("5号探针")<<tr("6号探针")<<tr("7号探针")<<tr("8号探针")
             <<tr("大探针2路")<<tr("大探针6路")<<tr("机头上限位")<<tr("机头下限位")<<tr("BTSR报警")<<tr("织物通过")<<tr("废纱2-4")<<tr("废纱6-8")
            <<tr("低气压")<<tr("低油压")<<tr("低油位")<<tr("出衣口")<<tr("KTF报警")<<tr("LGL")<<tr("")<<tr("")
           <<tr("针筒零位")<<tr("机头零位")<<tr("伺服报警")<<tr("点动")<<tr("运行")<<tr("停止")<<tr("急停")<<tr("");
    int index=0;
    for(int i=0;i<stringlist.length();i++)
    {
        label_text[i]=new QLabel(this);
        label_text[i]->setGeometry(40+index/12*260,20+(index%12)*35,130,31);
        label_text[i]->setText(stringlist[i]);

        tlb_state[i]=new QToolButton(this);
        tlb_state[i]->setGeometry(210+index/12*260,23+(index%12)*35,25,25);
        tlb_state[i]->setCheckable(true);
        tlb_state[i]->setEnabled(false);
        if(stringlist[i]==tr(""))
        {
            label_text[i]->hide();
            tlb_state[i]->hide();
        }
        else
            index++;
    }
}

/*实时刷新数据*/
void InputTesting::update_input_slot()
{
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)28||temp[0]==(char)10)
        {
            temp[0]=255;
            ui->pushButton_back->click();
        }
    }
    for(int i=0;i<4;i++)
    {
        value_state[i]=Ram.Receive(0x110+i);
        for(int j=0;j<8;j++)
        {
            int index=i*8+j;
            if(value_state[i]>>j&0x01==1)
            {
                if(index==13||index==16)
                    tlb_state[index]->setChecked(false);
                else
                    tlb_state[index]->setChecked(true);
            }
            else
            {
                if(index==13||index==16)
                    tlb_state[index]->setChecked(true);
                else
                    tlb_state[index]->setChecked(false);
            }
        }
    }
    //报警消除
    quint8 cancle=Ram.Receive(0x305);
    if(cancle==0x55)
    {
        Ram.Send(0xAA,0x305);
        Ram.Send(0xAA,0x303);
    }
}

//退出输入测试
void InputTesting::on_pushButton_back_clicked()
{
//    Ram.SendData[0]=0x15;
//    Ram.SendData[1]=0xA1;
//    Ram.SendData[2]=0xAA;
//    Ram.SendData[3]=0x00;
//    Ram.SendData[4]=0x00;
//    Ram.SendData[5]=0x00;
//    Ram.SendData[6]=0x00;
//    Ram.SendData[7]=0x00;
//    Ram.SendData[8]=0x00;
//    Ram.SendData[9]=0x00;
//    Ram.UI_to_RTC();
    Ram.SendData[0]=0x20;
    Ram.SendData[1]=0xA1;
    Ram.SendData[2]=0x0B;
    Ram.SendData[3]=0xAA;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    usleep(5000);
    Ram.to_TestingMain_slot();
    timer_input->stop();
    this->close();
}
