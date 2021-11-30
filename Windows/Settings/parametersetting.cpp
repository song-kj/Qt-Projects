#include "parametersetting.h"
#include "ui_parametersetting.h"
#include "QCom/qextserialport.h"
#include "runninginsetting.h"
#include "mymessagebox.h"
#include <QDebug>

extern QByteArray temp;
extern int startToCompare;

ParameterSetting::ParameterSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParameterSetting)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);
    button=new QPushButton();
    button_pre=new QPushButton();
    button_pre=button=ui->pinnumber_pb;
    digital_mark=0;
    timer_parasetting=new QTimer(this);
    connect(timer_parasetting, SIGNAL(timeout()),this,SLOT(update_slot()));
    timer_parasetting->start(10);

    initialForm();
    ReadFromFile();

    connect(ui->pinnumber_pb,SIGNAL(clicked()),this,SLOT(dealwithclicked()));
    connect(ui->ActuatorAdjust_pb,SIGNAL(clicked()),this,SLOT(dealwithclicked()));
//    connect(ui->FixedNeedle_pb,SIGNAL(clicked()),this,SLOT(dealwithclicked()));
    connect(ui->PointSpeed_pb,SIGNAL(clicked()),this,SLOT(dealwithclicked()));
    connect(ui->LimitedSpeed_pb,SIGNAL(clicked()),this,SLOT(dealwithclicked()));
    connect(ui->MaxSpeed_pb,SIGNAL(clicked()),this,SLOT(dealwithclicked()));
    connect(ui->Actuator_pb,SIGNAL(clicked(bool)),this,SLOT(dealwithclicked()));
    connect(ui->SumOutput_pb,SIGNAL(clicked()),this,SLOT(dealwithclicked()));
    connect(ui->injectiontimes_pb,SIGNAL(clicked()),this,SLOT(dealwithclicked()));
    connect(ui->InjectionInterval_pb,SIGNAL(clicked()),this,SLOT(dealwithclicked()));
    connect(ui->injectiontimes_pb_2,SIGNAL(clicked()),this,SLOT(dealwithclicked()));
    connect(ui->InjectionInterval_pb_2,SIGNAL(clicked()),this,SLOT(dealwithclicked()));
    connect(ui->valveStartTime_pb,SIGNAL(clicked()),this,SLOT(dealwithclicked()));
    connect(ui->valvePressRate_pb,SIGNAL(clicked()),this,SLOT(dealwithclicked()));
}

ParameterSetting::~ParameterSetting()
{
    delete ui;
}

void ParameterSetting::initialForm()
{
    degree_12[0]=48;
    degree_12[1]=236;
    degree_12[2]=498;
    degree_12[3]=686;
    degree_12[4]=948;
    degree_12[5]=1136;
    degree_12[6]=1398;
    degree_12[7]=1586;
    degree_12[8]=1848;
    degree_12[9]=2036;
    degree_12[10]=2298;
    degree_12[11]=2486;
    degree_12[12]=2748;
    degree_12[13]=2936;
    degree_12[14]=3198;
    degree_12[15]=3386;

    degree_13[0]=50;
    degree_13[1]=245;
    degree_13[2]=500;
    degree_13[3]=695;
    degree_13[4]=950;
    degree_13[5]=1145;
    degree_13[6]=1400;
    degree_13[7]=1595;
    degree_13[8]=1850;
    degree_13[9]=2045;
    degree_13[10]=2300;
    degree_13[11]=2495;
    degree_13[12]=2750;
    degree_13[13]=2945;
    degree_13[14]=3200;
    degree_13[15]=3395;

    degree_14[0]=71;
    degree_14[1]=242;
    degree_14[2]=521;
    degree_14[3]=692;
    degree_14[4]=971;
    degree_14[5]=1142;
    degree_14[6]=1421;
    degree_14[7]=1592;
    degree_14[8]=1871;
    degree_14[9]=2042;
    degree_14[10]=2321;
    degree_14[11]=2492;
    degree_14[12]=2771;
    degree_14[13]=2942;
    degree_14[14]=3221;
    degree_14[15]=3392;

    degree_15[0]=82;
    degree_15[1]=242;
    degree_15[2]=532;
    degree_15[3]=692;
    degree_15[4]=982;
    degree_15[5]=1142;
    degree_15[6]=1432;
    degree_15[7]=1592;
    degree_15[8]=1882;
    degree_15[9]=2042;
    degree_15[10]=2332;
    degree_15[11]=2492;
    degree_15[12]=2782;
    degree_15[13]=2942;
    degree_15[14]=3232;
    degree_15[15]=3392;

    degree_16[0]=90;
    degree_16[1]=240;
    degree_16[2]=540;
    degree_16[3]=690;
    degree_16[4]=990;
    degree_16[5]=1140;
    degree_16[6]=1440;
    degree_16[7]=1590;
    degree_16[8]=1890;
    degree_16[9]=2040;
    degree_16[10]=2340;
    degree_16[11]=2490;
    degree_16[12]=2790;
    degree_16[13]=2940;
    degree_16[14]=3240;
    degree_16[15]=3390;

    degree_17[0]=32;
    degree_17[1]=287;
    degree_17[2]=482;
    degree_17[3]=737;
    degree_17[4]=932;
    degree_17[5]=1187;
    degree_17[6]=1382;
    degree_17[7]=1637;
    degree_17[8]=1832;
    degree_17[9]=2087;
    degree_17[10]=2282;
    degree_17[11]=2537;
    degree_17[12]=2732;
    degree_17[13]=2987;
    degree_17[14]=3182;
    degree_17[15]=3437;

    degree_18[0]=53;
    degree_18[1]=320;
    degree_18[2]=503;
    degree_18[3]=770;
    degree_18[4]=953;
    degree_18[5]=1220;
    degree_18[6]=1403;
    degree_18[7]=1670;
    degree_18[8]=1853;
    degree_18[9]=2120;
    degree_18[10]=2303;
    degree_18[11]=2570;
    degree_18[12]=2753;
    degree_18[13]=3020;
    degree_18[14]=3203;
    degree_18[15]=3470;

    degree_19[0]=50;
    degree_19[1]=302;
    degree_19[2]=500;
    degree_19[3]=752;
    degree_19[4]=950;
    degree_19[5]=1202;
    degree_19[6]=1400;
    degree_19[7]=1652;
    degree_19[8]=1850;
    degree_19[9]=2102;
    degree_19[10]=2300;
    degree_19[11]=2552;
    degree_19[12]=2750;
    degree_19[13]=3002;
    degree_19[14]=3200;
    degree_19[15]=3452;

    degree_20[0]=56;
    degree_20[1]=309;
    degree_20[2]=506;
    degree_20[3]=759;
    degree_20[4]=956;
    degree_20[5]=1209;
    degree_20[6]=1406;
    degree_20[7]=1659;
    degree_20[8]=1856;
    degree_20[9]=2109;
    degree_20[10]=2306;
    degree_20[11]=2559;
    degree_20[12]=2756;
    degree_20[13]=3009;
    degree_20[14]=3206;
    degree_20[15]=3459;
}

void ParameterSetting::dealActuatorPosition()
{
    if(machine_Pars.fixed_needle==1)
    {
        for(int n=0;n<16;n++)//选针器位置设置 31-46
        {
            if(n%2==0)
            {
                machine_Pars.actuator_position[n]=1+(n/2)*(machine_Pars.totalneedles/8);
            }
            else
            {
                machine_Pars.actuator_position[n]=1+(n/2)*(machine_Pars.totalneedles/8)+machine_Pars.actuator_interval;
            }
        }
    }
    else
    {
        if(machineSize==12)
        {
            for(int i=0;i<16;i++)
                machine_Pars.actuator_position[i]=(degree_12[i]/3600.0)*machine_Pars.totalneedles;
        }
        else if(machineSize==13)
        {
            for(int i=0;i<16;i++)
                machine_Pars.actuator_position[i]=(degree_13[i]/3600.0)*machine_Pars.totalneedles;
        }
        else if(machineSize==14)
        {
            for(int i=0;i<16;i++)
                machine_Pars.actuator_position[i]=(degree_14[i]/3600.0)*machine_Pars.totalneedles;
        }
        else if(machineSize==15)
        {
            for(int i=0;i<16;i++)
                machine_Pars.actuator_position[i]=(degree_15[i]/3600.0)*machine_Pars.totalneedles;
        }
        else if(machineSize==16)
        {
            for(int i=0;i<16;i++)
                machine_Pars.actuator_position[i]=(degree_16[i]/3600.0)*machine_Pars.totalneedles;
        }
        else if(machineSize==17)
        {
            for(int i=0;i<16;i++)
                machine_Pars.actuator_position[i]=(degree_17[i]/3600.0)*machine_Pars.totalneedles;
        }
        else if(machineSize==18)
        {
            for(int i=0;i<16;i++)
                machine_Pars.actuator_position[i]=(degree_18[i]/3600.0)*machine_Pars.totalneedles;
        }
        else if(machineSize==19)
        {
            for(int i=0;i<16;i++)
                machine_Pars.actuator_position[i]=degree_19[i]*machine_Pars.totalneedles/3600.0;
        }
        else if(machineSize==20)
        {
            for(int i=0;i<16;i++)
                machine_Pars.actuator_position[i]=(degree_20[i]/3600.0)*machine_Pars.totalneedles;
        }
    }

}

void ParameterSetting::dealwithclicked()
{
    button=(QPushButton*)sender();
    if(button->toolTip().toInt()==1)
    {
        KeyBd *pKeyBd=new KeyBd;
        if(pKeyBd->exec()==QDialog::Accepted)
        {
            if(pKeyBd->inputstr=="")
            {
                ui->label_tips->setText("");
                temp[0]=255;
                MachTypeSelected *pMachTypeSelected=new MachTypeSelected;
                if(pMachTypeSelected->exec()==QDialog::Accepted)
                {
                    HideParaSetting();
                }
            }
            else
            {
                MyMessageBox *box = new MyMessageBox(tr("提示"),tr("密码错误！"),0);
                box->exec();
            }
        }
        delete pKeyBd;
        temp[0]=255;
    }
    else if(button->toolTip().toInt()==7)
    {
        KeyBd *pKeyBd=new KeyBd;
        if(pKeyBd->exec()==QDialog::Accepted)
        {
            if(pKeyBd->inputstr=="888888")
            {
                if(button->text()==tr("A型"))
                    button->setText(tr("B型"));
                else
                    button->setText(tr("A型"));
            }
            else
            {
                MyMessageBox *box = new MyMessageBox(tr("提示"),tr("密码错误！"),0);
                box->exec();
            }
        }
        delete pKeyBd;
        return;
    }
    if(button_pre==button)
    {
        button->setChecked(true);
    }
    else
    {
        button->setChecked(true);
        button_pre->setChecked(false);
    }
    button_pre=button;
}

void ParameterSetting::dealwithKey()
{
    QString number;
    if(this->isActiveWindow()&&temp.length()>=1)//left 29 right 39  up 33 down 35
    {
        if(temp[0]==(char)2)
        {
            temp[0]=255;
            ui->copy_pb->click();
        }
        if(temp[0]==(char)37)
        {
            temp[0]=255;
        }
        if(temp[0]==(char)28)
        {
            temp[0]=255;
            ui->back_Pra_pb->click();
        }
        if(temp[0]==(char)34)
        {
            temp[0]=255;
            ui->save_pb->click();
        }

        number=button->text();
        if(temp[0]==(char)1)
        {
            temp[0]=255;
            if(button->toolTip().toInt()==1)
            {
                ui->pinnumber_pb->click();
            }
        }
        if(button->toolTip().toInt()!=3)
        {
            if(temp[0]==(char)15)//+、-
            {
                temp[0]=255;
                if(button->toolTip().toInt()==2)
                {
                    if(number=="0")
                    {
                        button->setText("0");
                    }
                    else
                    {
                        qint16 adjust=button->text().toInt();
                        adjust=-adjust;
                        button->setText(QString::number(adjust));
                    }
                }

            }
            if(temp[0]==(char)27)
            {
                temp[0]=255;
                if(button->toolTip().toInt()!=1)
                {
                    button->setText("0");
                }
            }
            if(temp[0]==(char)12)
            {
                temp[0]=255;
                if(button->toolTip().toInt()!=1)
                {
                    number_answer(number);
                    if(number=="0")
                    {
                        button->setText("1");
                    }
                    else
                    {
                        button->setText(number.append("1"));
                    }
                }
            }
            if(temp[0]==(char)13)
            {
                temp[0]=255;
                if(button->toolTip().toInt()!=1)
                {
                    number_answer(number);
                    if(number=="0")
                    {
                        button->setText("4");
                    }
                    else
                    {
                        button->setText(number.append("4"));
                    }
                }
            }
            if(temp[0]==(char)14)
            {
                temp[0]=255;
                if(button->toolTip().toInt()!=1)
                {
                    number_answer(number);
                    if(number=="0")
                    {
                        button->setText("7");
                    }
                    else
                    {
                        button->setText(number.append("7"));
                    }
                }
            }
            if(temp[0]==(char)17)
            {
                temp[0]=255;
                if(button->toolTip().toInt()!=1)
                {
                    number_answer(number);
                    if(number=="0")
                    {
                        button->setText("2");
                    }
                    else
                    {
                        button->setText(number.append("2"));
                    }
                }
            }
            if(temp[0]==(char)18)
            {
                temp[0]=255;
                if(button->toolTip().toInt()!=1)
                {
                    number_answer(number);
                    if(number=="0")
                    {
                        button->setText("5");
                    }
                    else
                    {
                        button->setText(number.append("5"));
                    }
                }
            }
            if(temp[0]==(char)19)
            {
                temp[0]=255;
                if(button->toolTip().toInt()!=1)
                {
                    number_answer(number);
                    if(number=="0")
                    {
                        button->setText("8");

                    }
                    else
                    {
                        button->setText(number.append("8"));
                    }
                }
            }
            if(temp[0]==(char)20)
            {
                temp[0]=255;
                if(button->toolTip().toInt()!=1)
                {
                    number_answer(number);
                    if(number=="0")
                    {
                        button->setText("0");

                    }
                    else
                    {
                        button->setText(number.append("0"));
                    }
                }
            }
            if(temp[0]==(char)22)
            {
                temp[0]=255;
                if(button->toolTip().toInt()!=1)
                {
                    number_answer(number);
                    if(number=="0")
                    {
                        button->setText("3");
                    }
                    else
                    {
                        button->setText(number.append("3"));
                    }
                }
            }
            if(temp[0]==(char)23)
            {
                temp[0]=255;
                if(button->toolTip().toInt()!=1)
                {
                    number_answer(number);
                    if(number=="0")
                    {
                        button->setText("6");
                    }
                    else
                    {
                        button->setText(number.append("6"));
                    }
                }
            }
            if(temp[0]==(char)24)
            {
                temp[0]=255;
                if(button->toolTip().toInt()!=1)
                {
                    number_answer(number);
                    if(number=="0")
                    {
                        button->setText("9");
                    }
                    else
                    {
                        button->setText(number.append("9"));
                    }
                }
            }
        }

        if(temp[0]==(char)39)
        {
            temp[0]=255;
            digital_mark=0;
            if(button->toolTip().toInt()==1)
            {
                button_pre=ui->pinnumber_pb;
                ui->MaxSpeed_pb->click();
            }
            else if(button->toolTip().toInt()==3)
            {
                ui->SumOutput_pb->click();
            }
            else if(button->toolTip().toInt()==4)
            {
                ui->InjectionInterval_pb->click();
            }
            else if(button->toolTip().toInt()==5)
            {
                ui->injectiontimes_pb->click();
            }
            else if(button->toolTip().toInt()==11)//xinzeng
            {
                ui->InjectionInterval_pb_2->click();
            }
            else if(button->toolTip().toInt()==12)//xinzeng
            {
                ui->injectiontimes_pb_2->click();
            }
        }
        if(temp[0]==(char)29)
        {
            temp[0]=255;
            digital_mark=0;
            if(button->toolTip().toInt()==6)
            {
                button=ui->pinnumber_pb;
                ui->pinnumber_pb->setChecked(true);
                ui->MaxSpeed_pb->setChecked(false);
            }
            else if(button->toolTip().toInt()==8)
            {
                ui->FixedNeedle_pb->click();
            }

            else if(button->toolTip().toInt()==9)
            {
                ui->PointSpeed_pb->click();
            }

            else if(button->toolTip().toInt()==10)
            {
                ui->LimitedSpeed_pb->click();
            }
            else if(button->toolTip().toInt()==13)
            {
                ui->valveStartTime_pb->click();
            }
            else if(button->toolTip().toInt()==14)
            {
                ui->valvePressRate_pb->click();
            }
        }
        if(temp[0]==(char)33)
        {
            temp[0]=255;
            digital_mark=0;
            if(button->toolTip().toInt()==2)
            {
                button=ui->pinnumber_pb;
                ui->pinnumber_pb->setChecked(true);
                ui->ActuatorAdjust_pb->setChecked(false);
            }
            else if(button->toolTip().toInt()==3)
            {
                ui->ActuatorAdjust_pb->click();
            }
            else if(button->toolTip().toInt()==4)
            {
                ui->FixedNeedle_pb->click();
            }
            else if(button->toolTip().toInt()==5)
            {
                ui->PointSpeed_pb->click();
            }
            else if(button->toolTip().toInt()==11)
            {
                ui->LimitedSpeed_pb->click();
            }
            else if(button->toolTip().toInt()==12)
            {
                ui->valveStartTime_pb->click();
            }
            else if(button->toolTip().toInt()==6)
            {
                ui->valvePressRate_pb->click();
            }
            else if(button->toolTip().toInt()==8)
            {
                ui->MaxSpeed_pb->click();
            }
            else if(button->toolTip().toInt()==9)
            {
                ui->SumOutput_pb->click();
            }
            else if(button->toolTip().toInt()==10)
            {
                ui->InjectionInterval_pb->click();
            }
            else if(button->toolTip().toInt()==13)
            {
                ui->injectiontimes_pb->click();
            }
            else if(button->toolTip().toInt()==14)
            {
                ui->InjectionInterval_pb_2->click();
            }
        }
        if(temp[0]==(char)35)
        {
            temp[0]=255;
            digital_mark=0;
            if(button->toolTip().toInt()==10)
            {
                ui->valvePressRate_pb->click();
            }
            else if(button->toolTip().toInt()==9)
            {
                ui->injectiontimes_pb->click();
            }
            else if(button->toolTip().toInt()==8)
            {
                ui->InjectionInterval_pb->click();
            }
            else if(button->toolTip().toInt()==6)
            {
                ui->SumOutput_pb->click();
            }
            else if(button->toolTip().toInt()==13)
            {
                ui->injectiontimes_pb_2->click();
            }
            else if(button->toolTip().toInt()==11)
            {
                ui->valvePressRate_pb->click();
            }
            else if(button->toolTip().toInt()==11)
            {
                ui->MaxSpeed_pb->click();
            }
            else if(button->toolTip().toInt()==5)
            {
                ui->valveStartTime_pb->click();
            }

            else if(button->toolTip().toInt()==4)
            {
                ui->LimitedSpeed_pb->click();
            }

            else if(button->toolTip().toInt()==3)
            {
                ui->PointSpeed_pb->click();
            }
            else if(button->toolTip().toInt()==2)
            {
                ui->FixedNeedle_pb->click();
            }
            else if(button->toolTip().toInt()==1)
            {
                button_pre=ui->pinnumber_pb;
                ui->ActuatorAdjust_pb->click();
            }
        }
    }
}

void ParameterSetting::update_slot()
{
    dealwithKey();
}

void ParameterSetting::on_back_Pra_pb_clicked()
{
    timer_parasetting->stop();
    this->close();
}

//机型选择设置参数
void ParameterSetting::HideParaSetting()
{
    machine_Pars.actuator_num=16;
    if(machine_Pars.needletype==0)//12-1056
    {
        realtime_parameter.machineNeedleType="12/1056";
        machine_Pars.totalneedles=1056;
        machine_Pars.actuator_interval=55;
    }
    else if(machine_Pars.needletype==1)//13-1152
    {
       realtime_parameter.machineNeedleType="13/1152";
        machine_Pars.totalneedles=1152;
        machine_Pars.actuator_interval=62;
    }
    else if(machine_Pars.needletype==2)//14-1248
    {
        realtime_parameter.machineNeedleType="14/1248";
        machine_Pars.totalneedles=1248;
        machine_Pars.actuator_interval=59;
    }
    else if(machine_Pars.needletype==3)//14-1440
    {
        realtime_parameter.machineNeedleType="14/1440";
        machine_Pars.totalneedles=1440;
        machine_Pars.actuator_interval=68;
    }
    else if(machine_Pars.needletype==4)//15-1344
    {
        realtime_parameter.machineNeedleType="15/1344";
        machine_Pars.totalneedles=1344;
        machine_Pars.actuator_interval=60;
    }
    else if(machine_Pars.needletype==5)//16-1440
    {
        realtime_parameter.machineNeedleType="16/1440";
        machine_Pars.totalneedles=1440;
        machine_Pars.actuator_interval=60;
    }
    else if(machine_Pars.needletype==6)//17-1536
    {
        realtime_parameter.machineNeedleType="17/1536";
        machine_Pars.totalneedles=1536;
        machine_Pars.actuator_interval=109;
    }
    else if(machine_Pars.needletype==7)//15-1536
    {
        realtime_parameter.machineNeedleType="15/1536";
        machine_Pars.totalneedles=1536;
        machine_Pars.actuator_interval=69;
    }
    else if(machine_Pars.needletype==8)//18-1632
    {
        realtime_parameter.machineNeedleType="18/1632";
        machine_Pars.totalneedles=1632;
        machine_Pars.actuator_interval=121;
    }
    else if(machine_Pars.needletype==9)//13-1344
    {
        realtime_parameter.machineNeedleType="13/1344";
        machine_Pars.totalneedles=1344;
        machine_Pars.actuator_interval=73;
    }
    else if(machine_Pars.needletype==10)//16-1632
    {
        realtime_parameter.machineNeedleType="16/1632";
        machine_Pars.totalneedles=1632;
        machine_Pars.actuator_interval=68;
    }
    else if(machine_Pars.needletype==11)//17-1728
    {
        realtime_parameter.machineNeedleType="17/1728";
        machine_Pars.totalneedles=1728;
        machine_Pars.actuator_interval=126;
    }
    else if(machine_Pars.needletype==12)//19-1728
    {
        realtime_parameter.machineNeedleType="19/1728";
        machine_Pars.totalneedles=1728;
        machine_Pars.actuator_interval=120;
    }
    else if(machine_Pars.needletype==13)//14-1344
    {
        realtime_parameter.machineNeedleType="14/1344";
        machine_Pars.totalneedles=1344;
        machine_Pars.actuator_interval=64;
    }
    else if(machine_Pars.needletype==14)//14-1152
    {
        realtime_parameter.machineNeedleType="14/1152";
        machine_Pars.totalneedles=1152;
        machine_Pars.actuator_interval=55;
    }
    else if(machine_Pars.needletype==15)//13-1056
    {
        realtime_parameter.machineNeedleType="13/1056";
        machine_Pars.totalneedles=1056;
        machine_Pars.actuator_interval=57;
    }
    else if(machine_Pars.needletype==16)//12-960
    {
        realtime_parameter.machineNeedleType="12/960";
        machine_Pars.totalneedles=960;
        machine_Pars.actuator_interval=50;
    }
    else if(machine_Pars.needletype==17)//20-1824
    {
        realtime_parameter.machineNeedleType="20/1824";
        machine_Pars.totalneedles=1824;
        machine_Pars.actuator_interval=128;
    }
    ui->pinnumber_pb->setText(realtime_parameter.machineNeedleType);
    //机器寸数
    machineSize=realtime_parameter.machineNeedleType.left(2).toInt();
    dealActuatorPosition();
    startToCompare=1;
    para_operation.machine_par_save();
}

void ParameterSetting::ReadFromFile()
{
    //显示机型 寸数/针数
    ui->pinnumber_pb->setText(realtime_parameter.machineNeedleType);

    ui->ActuatorAdjust_pb->setText(QString::number(machine_Pars.actuator_adjust));
    //固定出针显示为奇数针或者偶数针
    if(machine_Pars.fixed_needle==0)
        ui->FixedNeedle_pb->setText(tr("奇数针"));
    else
        ui->FixedNeedle_pb->setText(tr("偶数针"));
    ui->PointSpeed_pb->setText(QString::number(machine_Pars.jog_speed));
    ui->LimitedSpeed_pb->setText(QString::number(machine_Pars.speed_limt));
    ui->MaxSpeed_pb->setText(QString::number(machine_Pars.max_speed));
    ui->Actuator_pb->setText(machine_Pars.actuatorType==0?"A型":"B型");
    ui->SumOutput_pb->setText(QString::number(machine_Pars.yield_setted));
    ui->InjectionInterval_pb->setText(QString::number(machine_Pars.oil_interval));
    ui->injectiontimes_pb->setText(QString::number(machine_Pars.oil_times));
    ui->valveStartTime_pb->setText(QString::number(machine_Pars.valve_start_time));
    ui->valvePressRate_pb->setText(QString::number(machine_Pars.valve_pressure));
    ui->InjectionInterval_pb_2->setText(QString::number(machine_Pars.oil_interval2));
    ui->injectiontimes_pb_2->setText(QString::number(machine_Pars.oil_times2));
}

void ParameterSetting::on_copy_pb_clicked()
{
    ParaOutIn *pParaOutIn=new ParaOutIn;
    pParaOutIn->exec();
    para_operation.machine_par_open();
    ReadFromFile();
}

//保存按钮
void ParameterSetting::on_save_pb_clicked()
{
//    if(ui->PointSpeed_pb->text().toUInt()==0)
//    {
//        MyMessageBox *box = new MyMessageBox(tr("提示"),tr("点动速度不能为0！"),0);
//        box->exec();
//        return;
//    }
//    if(ui->LimitedSpeed_pb->text().toUInt()==0)
//    {
//        MyMessageBox *box = new MyMessageBox(tr("提示"),tr("限速度值不能为0！"),0);
//        box->exec();
//        return;
//    }
//    if(ui->MaxSpeed_pb->text().toUInt()==0)
//    {
//        MyMessageBox *box = new MyMessageBox(tr("提示"),tr("最大速度不能为0！"),0);
//        box->exec();
//        return;
//    }
    machine_Pars.actuator_adjust=ui->ActuatorAdjust_pb->text().toUInt();
    if(ui->FixedNeedle_pb->text()==tr("奇数针"))
        machine_Pars.fixed_needle=0;
    else
        machine_Pars.fixed_needle=1;
    machine_Pars.jog_speed=ui->PointSpeed_pb->text().toUInt();
    machine_Pars.speed_limt=ui->LimitedSpeed_pb->text().toUInt();
    machine_Pars.max_speed=ui->MaxSpeed_pb->text().toUInt();
    if(ui->Actuator_pb->text()==tr("A型"))
        machine_Pars.actuatorType=0;
    else
        machine_Pars.actuatorType=1;
    machine_Pars.yield_setted=ui->SumOutput_pb->text().toUInt();
    machine_Pars.oil_interval=ui->InjectionInterval_pb->text().toUInt();
    machine_Pars.oil_times=ui->injectiontimes_pb->text().toUInt();
    machine_Pars.valve_start_time=ui->valveStartTime_pb->text().toUInt();
    machine_Pars.valve_pressure=ui->valvePressRate_pb->text().toUInt();
    machine_Pars.oil_interval2=ui->InjectionInterval_pb_2->text().toUInt();
    machine_Pars.oil_times2=ui->injectiontimes_pb_2->text().toUInt();
    dealActuatorPosition();
    para_operation.machine_par_save();
    ui->label_tips->setText(tr("保存成功"));
}

void ParameterSetting::number_answer(QString& n)
{
    if(digital_mark==0)
    {
        n="0";
        digital_mark=1;
    }
}

//磨合设置
void ParameterSetting::on_btn_runinsetting_clicked()
{
    KeyBd *pKeyBd=new KeyBd;
    if(pKeyBd->exec()==QDialog::Accepted)
    {
        if(pKeyBd->inputstr=="625000")
        {
            RunningInSetting *runin=new RunningInSetting;
            runin->exec();
        }
        else
        {
            MyMessageBox *box = new MyMessageBox(tr("提示"),tr("密码错误！"),0);
            box->exec();
        }
    }
    delete pKeyBd;
}

void ParameterSetting::on_FixedNeedle_pb_clicked()
{
    KeyBd *pKeyBd=new KeyBd;
    if(pKeyBd->exec()==QDialog::Accepted)
    {
        if(pKeyBd->inputstr=="888888")
        {
            if(ui->FixedNeedle_pb->text()==tr("奇数针"))
                ui->FixedNeedle_pb->setText(tr("偶数针"));
            else
                ui->FixedNeedle_pb->setText(tr("奇数针"));
        }
        else
        {
            MyMessageBox *box = new MyMessageBox(tr("提示"),tr("密码错误！"),0);
            box->exec();
        }
    }
    delete pKeyBd;
}
