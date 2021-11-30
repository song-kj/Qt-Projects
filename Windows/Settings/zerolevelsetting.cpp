#include "zerolevelsetting.h"
#include "ui_zerolevelsetting.h"
#include <QTime>
#include "Tool/KeyBoard/dialogcalculator.h"
#include "mymessagebox.h"

extern QByteArray temp;

ZeroLevelSetting::ZeroLevelSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ZeroLevelSetting)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);
    timer_zero=new QTimer(this);
    connect(timer_zero, SIGNAL(timeout()),this,SLOT(zero_slot()));
    Ram.Send(0xAA,0x300);

    //零位脉冲
    ui->zeropluse_pb->setText(QString::number(machine_Pars.zero_impluse));
    //选针零位针
    ui->zeroNeedleSetting_btn->setText(QString::number(machine_Pars.machine_zeor_needle));
    ui->currentpulse_lb->setText(QString::number(machine_Pars.zero_impluse));
    //总脉冲数
    ui->pulse_lb->setText(QString::number(machine_Pars.total_puls));
    ui->sumPluseSetting_btn->setText(QString::number(machine_Pars.total_puls));
    timer_zero->start(10);
}

ZeroLevelSetting::~ZeroLevelSetting()
{
    delete ui;
}


//返回
void ZeroLevelSetting::on_back_Zero_pb_clicked()
{
    Ram.SendData[0]=0x70;
    Ram.SendData[1]=0xA3;
    Ram.SendData[2]=0x00;
    Ram.SendData[3]=0x00;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    timer_zero->stop();
    this->close();
}

/*刷新槽*/
void ZeroLevelSetting::zero_slot()
{
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)37)
        {
        }
        if(temp[0]==(char)7)//自动
        {
            temp[0]=255;
            ui->auto_Zero_pb->click();
        }
        if(temp[0]==(char)8)
        {
            temp[0]=255;
            ui->pushbutton_set->click();
        }
        if(temp[0]==(char)10)//返回
        {
            temp[0]=255;
            ui->back_Zero_pb->click();
        }
        if(temp[0]==(char)9)//保存
        {
            temp[0]=255;
            ui->save_pb->click();
        }
        if(temp[0]==(char)28)//esc
        {
            temp[0]=255;
            on_back_Zero_pb_clicked();
        }
        if(temp[0]==(char)33)
        {
            temp[0]=255;
            ui->sumPluseSetting_btn->click();
        }
        if(temp[0]==(char)35)
        {
            temp[0]=255;
            ui->zeroNeedleSetting_btn->click();
        }
    }

    char plusenum_L=Ram.Receive(0x320);
    char plusenum_H=Ram.Receive(0x321);
    quint16  plusenum=(plusenum_H<<8)|plusenum_L;
    //当前脉冲
    ui->currentpulse_lb->setText(QString::number(plusenum));
    //当前针数
    ui->currentneedle_lb->setText(QString::number(realtime_parameter.curneedle));

    if(Ram.Receive(0x300)==0x55)
    {
        if(Ram.Receive(0x200)==0x70)
        {
            if((unsigned char)Ram.Receive(0x201)==0xA2)
            {
                Ram.Send(0xAA,0x300);
                unsigned char pluse_whole_L=Ram.Receive(0x202);
                unsigned char pluse_whole_H=Ram.Receive(0x203);
                unsigned char needle_whole_L=Ram.Receive(0x204);
                unsigned char needle_whole_H=Ram.Receive(0x205);

                quint16 sumP=(pluse_whole_H<<8)|pluse_whole_L;
                quint16 needle=(needle_whole_H<<8)|needle_whole_L;
                //总脉冲
                ui->pulse_lb->setText(QString::number(sumP));
                //总针数
                ui->needle_lb->setText(QString::number(needle));

                ui->sumPluseSetting_btn->setText(QString::number(sumP));
            }
        }
    }
}

/*自动*/
void ZeroLevelSetting::on_auto_Zero_pb_clicked()
{
    ui->auto_Zero_pb_2->setChecked(false);
    if(ui->auto_Zero_pb->isChecked())
    {
        Ram.SendData[0]=0x70;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x02;
        Ram.SendData[3]=0x00;
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
        ui->label->setText(QString::fromUtf8("自  动"));
    }
    else
    {
        Ram.SendData[0]=0x70;
        Ram.SendData[1]=0xA3;
        Ram.SendData[2]=0x00;
        Ram.SendData[3]=0x00;
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
        ui->label->setText(QString::fromUtf8("取  消"));
    }
}

//手动对零
void ZeroLevelSetting::on_auto_Zero_pb_2_clicked()
{
    ui->auto_Zero_pb->setChecked(false);
    if(ui->auto_Zero_pb_2->isChecked())
    {
        Ram.SendData[0]=0x70;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x01;
        Ram.SendData[3]=0x00;
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
        ui->label->setText(QString::fromUtf8("手  动"));
    }
    else
    {
        Ram.SendData[0]=0x70;
        Ram.SendData[1]=0xA3;
        Ram.SendData[2]=0x00;
        Ram.SendData[3]=0x00;
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
        ui->label->setText(QString::fromUtf8("取  消"));
    }}

//保存
void ZeroLevelSetting::on_save_pb_clicked()
{
    MyMessageBox *box=new MyMessageBox(tr("提示"),tr("是否保存当前零位设定？"),1);
    if(box->exec()==QDialog::Accepted)
    {
        if(ui->auto_Zero_pb->isChecked())
        {
            //零位脉冲赋值
            machine_Pars.zero_impluse=(quint16)ui->currentpulse_lb->text().toInt();
            ui->zeropluse_pb->setText(ui->currentpulse_lb->text());
            //零位针值赋值
            ui->zeroNeedleSetting_btn->setText(ui->currentneedle_lb->text());
        }
        //零位针值
        machine_Pars.machine_zeor_needle=(qint16)ui->zeroNeedleSetting_btn->text().toInt();
        //总脉冲
        machine_Pars.total_puls=(quint16)ui->sumPluseSetting_btn->text().toInt();

        para_operation.machine_par_save();

        Ram.SendData[0]=0x70;
        Ram.SendData[1]=0xA2;
        Ram.SendData[2]=0x00;
        Ram.SendData[3]=0x00;
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=quint8(machine_Pars.machine_zeor_needle);
        Ram.SendData[7]=quint8(machine_Pars.machine_zeor_needle>>8);
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
        zeroSetted=true;
        ui->label->setText(QString::fromUtf8("保存成功！"));
    }
}

//总脉冲设定
void ZeroLevelSetting::on_sumPluseSetting_btn_clicked()
{
//    if(ui->auto_Zero_pb->isChecked())
//    {
//        MyMessageBox *box=new MyMessageBox(tr("提示"),tr("自动设定下无法进行手动设定，请先取消自动设置！"),1);
//        box->exec();
//        return;
//    }
//    DialogCalculator *sr=new DialogCalculator;
//    keyMark=88;
//    sr->maxValue=65535;
//    sr->minValue=1;
//    if(sr->exec()==QDialog::Accepted)
//    {
//        ui->sumPluseSetting_btn->setText(sr->str);
//    }
//    delete sr;
//    keyMark=8;
}

//零位脉冲设定
void ZeroLevelSetting::on_zeroNeedleSetting_btn_clicked()
{
//    if(ui->auto_Zero_pb->isChecked())
//    {
//        MyMessageBox *box=new MyMessageBox(tr("提示"),tr("自动设定下无法进行收到设定，请先取消自动设置！"),1);
//        box->exec();
//        return;
//    }
//    DialogCalculator *sr=new DialogCalculator;
//    keyMark=88;
//    sr->maxValue=machine_Pars.totalneedles;
//    sr->minValue=1;
//    if(sr->exec()==QDialog::Accepted)
//    {
//        ui->zeroNeedleSetting_btn->setText(sr->str);
//    }
//    delete sr;
//    keyMark=8;
}

void ZeroLevelSetting::on_pushbutton_set_clicked()
{
    DialogCalculator *sr=new DialogCalculator;
    sr->maxValue=machine_Pars.totalneedles;
    sr->minValue=1;
    if(sr->exec()==QDialog::Accepted)
    {
        ui->zeroNeedleSetting_btn->setText(sr->str);
        //零位针值
        machine_Pars.machine_zeor_needle=(qint16)ui->zeroNeedleSetting_btn->text().toInt();
        //总脉冲
        machine_Pars.total_puls=(quint16)ui->sumPluseSetting_btn->text().toInt();

        para_operation.machine_par_save();

        Ram.SendData[0]=0x70;
        Ram.SendData[1]=0xA2;
        Ram.SendData[2]=0x00;
        Ram.SendData[3]=0x00;
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=quint8(machine_Pars.machine_zeor_needle);
        Ram.SendData[7]=quint8(machine_Pars.machine_zeor_needle>>8);
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
        zeroSetted=true;
        ui->label->setText(QString::fromUtf8("设定成功！"));
    }
    delete sr;
}

//机械零位偏差
void ZeroLevelSetting::on_btn_fixZeroOffset_clicked()
{
    DialogCalculator *sr=new DialogCalculator;
    sr->maxValue=machine_Pars.totalneedles;
    sr->minValue=1;
    if(sr->exec()==QDialog::Accepted)
    {
        ui->btn_fixZeroOffset->setText(QString::number(machine_Pars.fixZeroOffset));
        para_operation.machine_par_save();
    }
}
