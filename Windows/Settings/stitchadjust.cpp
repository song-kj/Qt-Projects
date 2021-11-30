#include "stitchadjust.h"
#include "ui_stitchadjust.h"
#include "myheader.h"
#include "Tool/KeyBoard/dialogcalculator.h"
#include "motorsaveornot.h"
#include "modifyco.h"

extern QByteArray temp;
StitchAdjust::StitchAdjust(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StitchAdjust)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    initialForm();
}

StitchAdjust::~StitchAdjust()
{
    delete ui;
}

//在设置界面进入压针调整界面 需要界面切换到编织界面
void StitchAdjust::setType(int t)
{
    type=t;
    Ram.to_PatternWoven_slot();
}

void StitchAdjust::initialForm()
{
    for(int i=0;i<8;i++)
    {
        motor_value[i]=machine_Pars.motor_adjust[i];
        motor_adjust[i]=0;
        label_name[i]=new QLabel(this);
        label_name[i]->setAlignment(Qt::AlignCenter);
        label_name[i]->setText(tr("第%1路电机微调").arg(i+1));
        label_name[i]->setGeometry(50+(i/4)*370,40+(i%4)*90,181,41);

        label_adjust[i]=new QLabel(this);
        label_adjust[i]->setAlignment(Qt::AlignCenter);
        label_adjust[i]->setText(tr("[%1]").arg(motor_adjust[i]));
        label_adjust[i]->setGeometry(310+(i/4)*370,40+(i%4)*90,71,41);

        btn_motors[i]=new QToolButton(this);
        btn_motors[i]->setCheckable(true);
        btn_motors[i]->setToolTip(QString::number(i));
        btn_motors[i]->setText(QString::number(motor_value[i]));
        btn_motors[i]->setGeometry(230+(i/4)*370,40+(i%4)*90,81,41);
        connect(btn_motors[i],SIGNAL(clicked(bool)),this,SLOT(motorclicked()));
    }
    cur_index=0;
    type=0;
    setall=false;
    modifyed=false;
    btn_motors[0]->setChecked(true);
    timer1=new QTimer(this);
    connect(timer1,SIGNAL(timeout()),this,SLOT(timeout()));
    timer1->start(20);
}

//参数下传
void StitchAdjust::downloadmotor()
{
    modifyed=true;
    int addr=0x500;
    for(int i=0;i<8;i++)
    {
        Ram.Send(quint8(motor_value[i]),addr);
        addr++;
        Ram.Send(quint8(motor_value[i]>>8),addr);
        addr++;
    }
    Ram.SendData[0]=0x20;
    Ram.SendData[1]=0xA2;
    Ram.SendData[2]=0x04;
    Ram.SendData[3]=0x01;
    Ram.SendData[4]=0xFF;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
}

//按键输入
void StitchAdjust::numberinput(QString num)
{
    return;
    if(setall)
    {

    }
    else
    {
        QString str=btn_motors[cur_index]->text();
        if(num=="del")
        {
            motor_value[cur_index]=0;
        }
        else if(num=="+-")
        {
            if(str!="0")
                motor_value[cur_index]*=-1;
        }
        else
        {
            str+=num;
            int value=str.toInt();
            if(value<=250||value>=-250)
            {
                motor_value[cur_index]=value;
            }
        }
        btn_motors[cur_index]->setText(QString::number(motor_value[cur_index]));
    }
}

//返回
void StitchAdjust::on_btn_back_clicked()
{
    if(modifyed)
    {
        MotorSaveOrNot *save=new MotorSaveOrNot;
        save->setLabelText(tr("参数已修改，是否保存？"));
        if(save->exec()==QDialog::Accepted)
        {
            ui->btn_save->click();
        }
        else
        {
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA2;
            Ram.SendData[2]=0x04;
            Ram.SendData[3]=0x01;
            Ram.SendData[4]=0xAA;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();
        }
    }
    else
    {
        Ram.SendData[0]=0x20;
        Ram.SendData[1]=0xA2;
        Ram.SendData[2]=0x04;
        Ram.SendData[3]=0x01;
        Ram.SendData[4]=0xAA;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();
    }
    //返回设置界面
    if(type==1)
        Ram.to_SettingMain_slot();
    this->close();
}

//点击单个电机
void StitchAdjust::motorclicked()
{
    if(setall)
    {
        setall=false;
        ui->btn_setall->setChecked(setall);
    }
    int index=((QToolButton*)sender())->toolTip().toInt();
    btn_motors[index]->setChecked(true);
    if(index==cur_index)
    {
//        DialogCalculator *dia=new DialogCalculator;
//        dia->setFlag(true);
//        dia->minValue=-250;
//        dia->maxValue=250;
//        if(dia->exec()==QDialog::Accepted)
//        {
//            int value=dia->str.toInt();
//            motor_adjust[index]=value-motor_value[index];
//            motor_value[index]=dia->str.toInt();
//            btn_motors[index]->setText(QString::number(motor_value[index]));
//            label_adjust[index]->setText(tr("[%1]").arg(QString::number(motor_adjust[index])));
//            modifyed=true;
//        }
//        delete dia;
    }
    cur_index=index;
    for(int i=0;i<8;i++)
    {
        if(i!=cur_index)
            btn_motors[i]->setChecked(false);
    }
}

//++
void StitchAdjust::on_btn_add_clicked()
{
    if(setall)
    {
        for(int i=0;i<8;i++)
        {
            int value=motor_value[i];
            if(value<250)
            {
                value++;
                motor_adjust[i]++;
            }
            motor_value[i]=value;
            btn_motors[i]->setText(QString::number(value));
            label_adjust[i]->setText(tr("[%1]").arg(motor_adjust[i]));
        }
        downloadmotor();
    }
    else
    {
        int value=motor_value[cur_index];
        if(value<250)
        {
            value++;
            motor_adjust[cur_index]++;
            motor_value[cur_index]=value;
            btn_motors[cur_index]->setText(QString::number(value));
            label_adjust[cur_index]->setText(tr("[%1]").arg(motor_adjust[cur_index]));
            downloadmotor();
        }
    }
}

//--
void StitchAdjust::on_btn_sub_clicked()
{
    if(setall)
    {
        for(int i=0;i<8;i++)
        {
            int value=motor_value[i];
            if(value>-250)
            {
                value--;
                motor_adjust[i]--;
            }
            motor_value[i]=value;
            btn_motors[i]->setText(QString::number(value));
            label_adjust[i]->setText(tr("[%1]").arg(motor_adjust[i]));
        }
        downloadmotor();
    }
    else
    {
        int value=motor_value[cur_index];
        if(value>-250)
        {
            value--;
            motor_adjust[cur_index]--;
            motor_value[cur_index]=value;
            btn_motors[cur_index]->setText(QString::number(value));
            label_adjust[cur_index]->setText(tr("[%1]").arg(motor_adjust[cur_index]));
            downloadmotor();
        }
    }
}

//全设
void StitchAdjust::on_btn_setall_clicked()
{
    setall=ui->btn_setall->isChecked();
    if(setall)
    {
        for(int i=0;i<8;i++)
            btn_motors[i]->setChecked(true);
    }
}

//保存
void StitchAdjust::on_btn_save_clicked()
{
    for(int i=0;i<8;i++)
    {
        machine_Pars.motor_adjust[i]=motor_value[i];
        motor_adjust[i]=0;
        label_adjust[i]->setText(tr("[%1]").arg(motor_adjust[i]));
    }
    para_operation.machine_par_save();
    modifyed=false;
    int addr=0x500;
    for(int i=0;i<8;i++)
    {
        Ram.Send(quint8(motor_value[i]),addr);
        addr++;
        Ram.Send(quint8(motor_value[i]>>8),addr);
        addr++;
    }
    Ram.SendData[0]=0x20;
    Ram.SendData[1]=0xA2;
    Ram.SendData[2]=0x04;
    Ram.SendData[3]=0x01;
    Ram.SendData[4]=0x55;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    ui->btn_save->setText(tr("保\n存\n成\n功"));
    ui->btn_save->setCheckable(true);
    ui->btn_save->setChecked(true);
    QTimer::singleShot(500,this,SLOT(saveok()));
}

void StitchAdjust::timeout()
{
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        quint8 key=temp[0];
        temp.clear();
        switch (key) {
        case 2:
            ui->btn_setall->click();
            break;
        case 3:
            ui->btn_add->click();
            break;
        case 4:
            ui->btn_sub->click();
            break;
        case 5:
            ui->btn_chainstitch->click();
            break;
        case 9:
            ui->btn_save->click();
            break;
        case 10:
        case 28:
            ui->btn_back->click();
            break;
        case 12:
            numberinput("1");
            break;
        case 13:
            numberinput("4");
            break;
        case 14:
            numberinput("7");
            break;
        case 15:
            numberinput("+-");
            break;
        case 17:
            numberinput("2");
            break;
        case 18:
            numberinput("5");
            break;
        case 19:
            numberinput("8");
            break;
        case 20:
            numberinput("0");
            break;
        case 22:
            numberinput("3");
            break;
        case 23:
            numberinput("6");
            break;
        case 24:
            numberinput("9");
            break;
        case 26:
            numberinput("del");
            break;
        case 71:
            b_headerUp_Down=tr("升机头...");
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA2;
            Ram.SendData[2]=0x01;
            Ram.SendData[3]=0x55;
            Ram.SendData[4]=0x00;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();
            break;
        case 11:
            b_headerUp_Down=tr("升机头取消！");
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA2;
            Ram.SendData[2]=0x01;
            Ram.SendData[3]=0xaa;
            Ram.SendData[4]=0x00;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();
            break;
        case 76:
            b_headerUp_Down=tr("降机头...");
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA2;
            Ram.SendData[2]=0x02;
            Ram.SendData[3]=0x55;
            Ram.SendData[4]=0x00;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();
            break;
        case 16:
            b_headerUp_Down=tr("降机头取消！");
            Ram.SendData[0]=0x20;
            Ram.SendData[1]=0xA2;
            Ram.SendData[2]=0x02;
            Ram.SendData[3]=0xaa;
            Ram.SendData[4]=0x00;
            Ram.SendData[5]=0x00;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();
            break;
        default:
            break;
        }
    }
}

void StitchAdjust::saveok()
{
    ui->btn_save->setText(tr("参\n数\n保\n存"));
    ui->btn_save->setCheckable(false);
}

//进入链条压针界面
void StitchAdjust::on_btn_chainstitch_clicked()
{
    if(realtime_parameter.isRunnging==false)
    {
        ModifyCO *co=new ModifyCO;
        co->settype(2);
        co->exec();
    }
}
