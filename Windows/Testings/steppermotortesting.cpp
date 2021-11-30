#include "steppermotortesting.h"
#include "ui_steppermotortesting.h"

extern QByteArray temp;
StepperMotorTesting::StepperMotorTesting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StepperMotorTesting)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);
    revwesal=false;
    y=70;
    mark=0;
    display=0;
    sum_up=0;
    mark_up=0;
    dislabel_up=new QPushButton();
    sum_down=0;
    mark_down=0;
    speed_interval=5;
    dislabel_down=new QPushButton();
    display_lb=new QPushButton();
    label=new QPushButton();
    label->setToolTip(QString::fromUtf8("1"));
    sum_whole=0;
    digital_mark=0;
    ui->showtime_pb->setStyleSheet("QPushButton{border-radius:10; border-width:0;border-style:outset;background: transparent;font-size:20px;color:red}");
    for(int i=0;i<9;i++)
    {
        motor_edit[i]=new QLabel(this);
        steppermotor_1_lb[i]=new QPushButton(this);
        motor_edit[i]->setToolTip(tr("%1").arg(i+1));
        steppermotor_1_lb[i]->setToolTip(tr("%1").arg(i+1));
        motor_edit[i]->setText(tr("0"));
        connect(steppermotor_1_lb[i],SIGNAL(clicked()),this,SLOT(deal_steppermotor()));
        if(i==8)
        {
            motor_edit[i]->setGeometry(70,350,50,27);
            steppermotor_1_lb[i]->setGeometry(70,280,50,50);
            steppermotor_1_lb[i]->setStyleSheet("QPushButton{border-image: url(:/img/StepMotorUnselect.bmp)}");
            motor_edit[i]->setStyleSheet("QLabel{font-size:20px;color:white;border:1px solid white}");
        }

        else
        {
            motor_edit[i]->setGeometry(y,190,50,27);
            steppermotor_1_lb[i]->setGeometry(y,120,50,50);
            steppermotor_1_lb[i]->setStyleSheet("QPushButton{border-image: url(:/img/StepMotorUnselect.bmp)}");
            motor_edit[i]->setStyleSheet("QLabel{font-size:20px;color:white;border:1px solid white}");
        }
        motor_edit[0]->setStyleSheet("QLabel{font-size:20px;color:red;border:1px solid yellow}");
        display_lb=label=steppermotor_1_lb[0];
        steppermotor_1_lb[0]->setStyleSheet("QPushButton{border-image: url(:/img/StepMotorSelect.bmp)}");
        trigger=false;
        editstate=1;
        y=y+85;
    }

    timer_stepper=new QTimer(this);
    connect(timer_stepper, SIGNAL(timeout()),this,SLOT(update_slot()));
    timer_stepper->start(10);
}
StepperMotorTesting::~StepperMotorTesting()
{
    delete ui;
}
/**识别点击的电机号*/
void StepperMotorTesting::deal_steppermotor()
{
    label=(QPushButton*)sender();
    if(mark==0)
        display_lb=label;
    if(display_lb==label)
    {
        if(mark==0)
        {
            if(label!=steppermotor_1_lb[0])
            {
                steppermotor_1_lb[0]->setStyleSheet("QPushButton{border-image: url(:/img/StepMotorUnselect.bmp)}");
                motor_edit[0]->setStyleSheet("QLabel{font-size:20px;color:white;border:1px solid white}");
                trigger=true;
            }
        }
        if(trigger)
        {
            label->setStyleSheet("QPushButton{border-image: url(:/img/StepMotorSelect.bmp)}");
            motor_edit[label->toolTip().toInt()-1]->setStyleSheet("QLabel{font-size:20px;color:red;border:1px solid yellow}");
            trigger=false;
            editstate=1;//ke bian ji
        }
        else
        {
            label->setStyleSheet("QPushButton{border-image: url(:/img/StepMotorUnselect.bmp)}");
            motor_edit[label->toolTip().toInt()-1]->setStyleSheet("QLabel{font-size:20px;color:white;border:1px solid yellow}");
            trigger=true;
            editstate=0;   // bu ke bian ji
        }
    }
    else
    {
        steppermotor_1_lb[display-1]->setStyleSheet("QPushButton{border-image: url(:/img/StepMotorUnselect.bmp)}");
        label->setStyleSheet("QPushButton{border-image: url(:/img/StepMotorSelect.bmp)}");
        motor_edit[label->toolTip().toInt()-1]->setStyleSheet("QLabel{font-size:20px;color:red;border:1px solid yellow}");
        motor_edit[display_lb->toolTip().toInt()-1]->setStyleSheet("QLabel{font-size:20px;color:white;border:1px solid white}");
        trigger=false;
        editstate=1;
    }
    display_lb=label;
    display=label->toolTip().toInt();
    mark++;
}
/*速度加*/
void StepperMotorTesting::on_speedup_Stp_pb_clicked()
{
    //ui->showtime_pb->setText(QString::fromUtf8("加速..."));
    if(label->toolTip().toInt()==10)
    {
        QMessageBox::warning(this,QString::fromUtf8("提  示"),QString::fromUtf8("请先选择一路"),QMessageBox::Yes);
    }
    else
    {
        if(editstate==1)
        {
            if(mark_up==0)
                dislabel_up=label;
            if(dislabel_up==label)
            {
                if(sum_up<400)
                {
                    sum_up=sum_up+speed_interval;
                    motor_edit[label->toolTip().toInt()-1]->setText(tr("%1").arg(sum_up));
                }
                else
                {
                    motor_edit[label->toolTip().toInt()-1]->setText(tr("%1").arg(0));
                    sum_up=0;
                }
            }
            else
            {
                //sum_up=5;
                sum_up=motor_edit[label->toolTip().toInt()-1]->text().toInt()+speed_interval;
                motor_edit[label->toolTip().toInt()-1]->setText(tr("%1").arg(sum_up));

            }
            unsigned char sum_up_H=(sum_up&0xff00)>>8;
            unsigned char sum_up_L=sum_up&0x00ff;
            Ram.SendData[0]=0x10;
            Ram.SendData[1]=0xA3;
            Ram.SendData[2]=0x01;
            Ram.SendData[3]=label->toolTip().toInt();
            Ram.SendData[4]=sum_up_L;
            Ram.SendData[5]=sum_up_H;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();
            dislabel_up=label;
            mark_up++;
        }
    }
}
/*减速*/
void StepperMotorTesting::on_slowdown_Stp_pb_clicked()
{
    //  ui->showtime_pb->setText(QString::fromUtf8("减速..."));
    if(label->toolTip().toInt()==10)
    {
        QMessageBox::warning(this,QString::fromUtf8("提  示"),QString::fromUtf8("请先选择一路"),QMessageBox::Yes);
    }
    else
    {
        if(editstate==1)
        {
            if(mark_down==0)
                dislabel_down=label;
            if(dislabel_down==label)
            {
                if(sum_up>-400)
                {
                    sum_up=sum_up-speed_interval;//sudu tiaoji
                    motor_edit[label->toolTip().toInt()-1]->setText(tr("%1").arg(sum_up));
                }
                else
                {
                    motor_edit[label->toolTip().toInt()-1]->setText(tr("%1").arg(0));
                    sum_up=0;
                }
            }
            else
            {
                // sum_up=-5;
                sum_up=motor_edit[label->toolTip().toInt()-1]->text().toInt()-speed_interval;
                motor_edit[label->toolTip().toInt()-1]->setText(tr("%1").arg(sum_up));
            }
            unsigned char sum_up_H=(sum_up&0xff00)>>8;
            unsigned char sum_up_L=sum_up&0x00ff;
            Ram.SendData[0]=0x10;
            Ram.SendData[1]=0xA3;
            Ram.SendData[2]=0x01;
            Ram.SendData[3]=label->toolTip().toInt();
            Ram.SendData[4]=sum_up_L;
            Ram.SendData[5]=sum_up_H;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();
            dislabel_down=label;
            mark_down++;
        }
    }
}
/*全位跑*/
void StepperMotorTesting::on_fullrun_Stp_pb_clicked()
{
    ui->showtime_pb->setText(QString::fromUtf8("全位跑..."));
    for(int l=0;l<9;l++)
    {
        motor_edit[l]->setStyleSheet("QLabel{font-size:20px;color:red;border:1px solid yellow}");
    }
    if(sum_whole<400&&sum_whole>-400)
    {
        if(revwesal)//fan zhuan
            sum_whole=sum_whole-20;
        else
            sum_whole=sum_whole+20;
    }

    else
        sum_whole=0;
    for(int i=0;i<8;i++)
    {
        motor_edit[i]->setText(tr("%1").arg(sum_whole));
    }
    sum_up=sum_whole;
    unsigned char sum_whole_H=(sum_whole&0xff00)>>8;
    unsigned char sum_whole_L=sum_whole&0x00ff;
    Ram.SendData[0]=0x10;
    Ram.SendData[1]=0xA3;
    Ram.SendData[2]=0x02;
    Ram.SendData[3]=0xff;
    Ram.SendData[4]=sum_whole_L;
    Ram.SendData[5]=sum_whole_H;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();

}
/*全归零*/
void StepperMotorTesting::on_fullzero_Stp_pb_clicked()
{
    ui->showtime_pb->setText(QString::fromUtf8("全归零..."));
    for(int l=0;l<9;l++)
    {
        motor_edit[l]->setStyleSheet("QLabel{font-size:20px;color:red;border:1px solid yellow}");
    }
    for(int i=0;i<8;i++)
    {
        motor_edit[i]->setText(tr("%1").arg(0));
    }
    sum_up=sum_whole=0;
    Ram.SendData[0]=0x10;
    Ram.SendData[1]=0xA1;
    Ram.SendData[2]=0x02;
    Ram.SendData[3]=0xff;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
}

//刷新槽
void StepperMotorTesting::update_slot()
{
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)37)
        {

        }
        QString number= ui->speedsetting_pb->text();
        if(temp[0]==(char)27)
        {
            temp[0]=255;
            ui->speedsetting_pb->setText("0");
            speed_interval=ui->speedsetting_pb->text().toInt();

        }
        if(temp[0]==(char)12)
        {
            temp[0]=255;
            number_answer(number);
            if(number=="0")
            {
                ui->speedsetting_pb->setText("1");
                speed_interval=ui->speedsetting_pb->text().toInt();
            }
            else
            {
                ui->speedsetting_pb->setText(number.append("1"));
                speed_interval=ui->speedsetting_pb->text().toInt();
            }

        }
        if(temp[0]==(char)13)
        {
            temp[0]=255;
            number_answer(number);
            if(number=="0")
            {
                ui->speedsetting_pb->setText("4");
                speed_interval=ui->speedsetting_pb->text().toInt();
            }
            else
            {
                ui->speedsetting_pb->setText(number.append("4"));
                speed_interval=ui->speedsetting_pb->text().toInt();
            }

        }
        if(temp[0]==(char)14)
        {
            temp[0]=255;
            number_answer(number);
            if(number=="0")
            {
                ui->speedsetting_pb->setText("7");
                speed_interval=ui->speedsetting_pb->text().toInt();
            }
            else
            {
                ui->speedsetting_pb->setText(number.append("7"));
                speed_interval=ui->speedsetting_pb->text().toInt();
            }
        }
        if(temp[0]==(char)17)
        {
            temp[0]=255;
            number_answer(number);
            if(number=="0")
            {
                ui->speedsetting_pb->setText("2");
                speed_interval=ui->speedsetting_pb->text().toInt();

            }
            else
            {
                ui->speedsetting_pb->setText(number.append("2"));
                speed_interval=ui->speedsetting_pb->text().toInt();
            }

        }
        if(temp[0]==(char)18)
        {
            temp[0]=255;
            number_answer(number);
            if(number=="0")
            {
                ui->speedsetting_pb->setText("5");
                speed_interval=ui->speedsetting_pb->text().toInt();

            }
            else
            {
                ui->speedsetting_pb->setText(number.append("5"));
                speed_interval=ui->speedsetting_pb->text().toInt();
            }

        }
        if(temp[0]==(char)19)
        {
            temp[0]=255;
            number_answer(number);
            if(number=="0")
            {
                ui->speedsetting_pb->setText("8");
                speed_interval=ui->speedsetting_pb->text().toInt();

            }
            else
            {
                ui->speedsetting_pb->setText(number.append("8"));
                speed_interval=ui->speedsetting_pb->text().toInt();
            }

        }
        if(temp[0]==(char)20)
        {
            temp[0]=255;
            number_answer(number);
            if(number=="0")
            {
                ui->speedsetting_pb->setText("0");
                speed_interval=ui->speedsetting_pb->text().toInt();

            }
            else
            {
                ui->speedsetting_pb->setText(number.append("0"));
                speed_interval=ui->speedsetting_pb->text().toInt();
            }

        }
        if(temp[0]==(char)22)
        {
            temp[0]=255;
            number_answer(number);
            if(number=="0")
            {
                ui->speedsetting_pb->setText("3");
                speed_interval=ui->speedsetting_pb->text().toInt();

            }
            else
            {
                ui->speedsetting_pb->setText(number.append("3"));
                speed_interval=ui->speedsetting_pb->text().toInt();
            }

        }
        if(temp[0]==(char)23)
        {
            temp[0]=255;
            number_answer(number);
            if(number=="0")
            {
                ui->speedsetting_pb->setText("6");
                speed_interval=ui->speedsetting_pb->text().toInt();

            }
            else
            {
                ui->speedsetting_pb->setText(number.append("6"));
                speed_interval=ui->speedsetting_pb->text().toInt();
            }
        }
        if(temp[0]==(char)24)
        {
            temp[0]=255;
            number_answer(number);
            if(number=="0")
            {
                ui->speedsetting_pb->setText("9");
                speed_interval=ui->speedsetting_pb->text().toInt();

            }
            else
            {
                ui->speedsetting_pb->setText(number.append("9"));
                speed_interval=ui->speedsetting_pb->text().toInt();
            }
        }
        if(temp[0]==(char)1)
        {
            temp[0]=255;
            ui->fullrun_Stp_pb->click();
        }
        if(temp[0]==(char)2)
        {
            temp[0]=255;
            ui->fullzero_Stp_pb->click();
        }
        if(temp[0]==(char)3)
        {
            temp[0]=255;
            ui->singlezero_Stp_pb->click();
        }
        if(temp[0]==(char)4)
        {
            temp[0]=255;
            ui->unitrun_Stp_pb->click();
        }
        if(temp[0]==(char)6)
        {
            temp[0]=255;
            ui->speedup_Stp_pb->click();
        }
        if(temp[0]==(char)7)
        {
            temp[0]=255;
            ui->slowdown_Stp_pb->click();
        }
        if(temp[0]==(char)8)
        {
            temp[0]=255;
            ui->revwesal_Stp_pb->click();
        }
        if(temp[0]==(char)9)
        {
            temp[0]=255;
            on_back_Stp_pb_clicked();
        }
        if(temp[0]==(char)28)
        {
            temp[0]=255;
            ui->back_Stp_pb->click();
        }
        if(temp[0]==(char)39)
        {
            temp[0]=255;
            if(label->toolTip().toInt()<9)
            {
                int ID=label->toolTip().toInt();
                steppermotor_1_lb[ID]->click();
            }
        }
        if(temp[0]==(char)29)
        {
            temp[0]=255;
            if(label->toolTip().toInt()>1)
            {
                int ID=label->toolTip().toInt();
                steppermotor_1_lb[ID-2]->click();
            }

        }


    }
    //}
}

/**单位跑**/
void StepperMotorTesting::on_unitrun_Stp_pb_clicked()
{
    ui->showtime_pb->setText(QString::fromUtf8("单位跑..."));
    for(int l=0;l<9;l++)
    {
        motor_edit[l]->setStyleSheet("QLabel{font-size:20px;color:white;border:1px solid white}");
    }
    motor_edit[label->toolTip().toInt()-1]->setStyleSheet("QLabel{font-size:20px;color:red;border:1px solid yellow}");
    if(label->toolTip().toInt()==10)
    {
        QMessageBox::warning(this,QString::fromUtf8("提  示"),QString::fromUtf8("请先选择一路"),QMessageBox::Yes);
    }
    else
    {
        if(editstate==1)
        {
            if(sum_up<400&&sum_up>-400)
            {
                sum_up=motor_edit[label->toolTip().toInt()-1]->text().toInt();
                if(revwesal)
                    sum_up=sum_up-20;
                else
                    sum_up=sum_up+20;
            }
            else
                sum_up=0;
            motor_edit[label->toolTip().toInt()-1]->setText(tr("%1").arg(sum_up));
            unsigned char sum_up_H=(sum_up&0xff00)>>8;
            unsigned char sum_up_L=sum_up&0x00ff;
            //ui->lineEdit->setText(QString::number(Ram.Receive(0x301)));
            Ram.SendData[0]=0x10;
            Ram.SendData[1]=0xA3;
            Ram.SendData[2]=0x01;
            Ram.SendData[3]=label->toolTip().toInt();
            Ram.SendData[4]=sum_up_L;
            Ram.SendData[5]=sum_up_H;
            Ram.SendData[6]=0x00;
            Ram.SendData[7]=0x00;
            Ram.SendData[8]=0x00;
            Ram.SendData[9]=0x00;
            Ram.UI_to_RTC();
        }
    }

}

/*单归零*/
void StepperMotorTesting::on_singlezero_Stp_pb_clicked()
{
    ui->showtime_pb->setText(QString::fromUtf8("单归零..."));
    for(int l=0;l<9;l++)
    {
        motor_edit[l]->setStyleSheet("QLabel{font-size:20px;color:white;border:1px solid white}");
    }
    motor_edit[label->toolTip().toInt()-1]->setStyleSheet("QLabel{font-size:20px;color:red;border:1px solid yellow}");
    if(label->toolTip().toInt()==10)
    {
        QMessageBox::warning(this,QString::fromUtf8("提  示"),QString::fromUtf8("请先选择一路"),QMessageBox::Yes);
    }
    else
    {
        if(editstate==1)
        {
            motor_edit[label->toolTip().toInt()-1]->setText(tr("%1").arg(0));
            sum_up=0;
        }
        Ram.SendData[0]=0x10;
        Ram.SendData[1]=0xA1;
        Ram.SendData[2]=0x01;
        Ram.SendData[3]=label->toolTip().toInt();
        Ram.SendData[4]=0x00;
        Ram.SendData[5]=0x00;
        Ram.SendData[6]=0x00;
        Ram.SendData[7]=0x00;
        Ram.SendData[8]=0x00;
        Ram.SendData[9]=0x00;
        Ram.UI_to_RTC();



    }
}

/**返回回到编织状态**/
void StepperMotorTesting::on_back_Stp_pb_clicked()
{
    timer_stepper->stop();
    Ram.SendData[0]=0x10;
    Ram.SendData[1]=0xAA;
    Ram.SendData[2]=0x00;
    Ram.SendData[3]=0x00;
    Ram.SendData[4]=0x00;
    Ram.SendData[5]=0x00;
    Ram.SendData[6]=0x00;
    Ram.SendData[7]=0x00;
    Ram.SendData[8]=0x00;
    Ram.SendData[9]=0x00;
    Ram.UI_to_RTC();
    this->close();
}

void StepperMotorTesting::on_speedsetting_pb_clicked()
{

}

void StepperMotorTesting::RreceiveData(QString speed)
{
    speed_interval=speed.toInt();
}

void StepperMotorTesting::on_revwesal_Stp_pb_clicked()
{
    revwesal=ui->revwesal_Stp_pb->isChecked();
}

void StepperMotorTesting::number_answer(QString& n)
{
    if(digital_mark==0)
    {
        n="0";
        digital_mark=1;
    }
}
