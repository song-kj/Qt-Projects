#include "testingmain.h"
#include "ui_testingmain.h"

extern QByteArray temp;
TestingMain::TestingMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestingMain)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    Ram.to_TestingMain_slot();
    timer_testmain=new QTimer(this);
    timer_testmain->start(1);
    connect(timer_testmain,SIGNAL(timeout()),this,SLOT(update_slot()));//timeoutslot()为自定义槽
}

TestingMain::~TestingMain()
{
    delete ui;
}

void TestingMain::update_slot()
{
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)37)
        {

        }
        if(temp[0]==(char)12)
        {
            temp[0]=255;
            ui->toolButton_airvalve->click();
        }
        else if(temp[0]==(char)17)
        {
            temp[0]=255;
            ui->toolButton_actuator->click();
        }
        else if(temp[0]==(char)22)
        {
            temp[0]=255;
            ui->toolButton_stepmotor->click();
        }
        else if(temp[0]==(char)13)
        {
            temp[0]=255;
            ui->toolButton_input->click();
        }
        else if(temp[0]==(char)18)
        {
            temp[0]=255;
            ui->toolButton_output->click();
        }
        else if(temp[0]==(char)28||temp[0]==(char)23)
        {
            temp[0]=255;
            ui->toolButton_back->click();
        }
    }
}


//气阀测试
void TestingMain::on_toolButton_airvalve_clicked()
{
    Ram.SendData[0]=0x11;
    Ram.SendData[1]=0xA0;
    Ram.SendData[2]=0x01;
    Ram.SendData[3]=0x55;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    AirValveTesting *pAirValveTesting=new AirValveTesting;
    pAirValveTesting->exec();
}

//选针器测试
void TestingMain::on_toolButton_actuator_clicked()
{
    ActuatorTesting *pActuatorTesting=new ActuatorTesting;
    pActuatorTesting->exec();
}

//步进电机测试
void TestingMain::on_toolButton_stepmotor_clicked()
{
    StepperMotorTesting *pStepperMotorTesting=new StepperMotorTesting;
    pStepperMotorTesting->exec();
}

//输入测试
void TestingMain::on_toolButton_input_clicked()
{
    InputTesting *pInputTesting=new InputTesting;
    pInputTesting->exec();
}

//输出测试
void TestingMain::on_toolButton_output_clicked()
{
    OutputTesting *pOutputTesting=new OutputTesting;
    pOutputTesting->exec();
}

//返回
void TestingMain::on_toolButton_back_clicked()
{
    timer_testmain->stop();
    this->close();
}
