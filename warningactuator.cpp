#include "warningactuator.h"
#include "ui_warningactuator.h"

WarningActuator::WarningActuator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WarningActuator)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width()-this->width())/2,(deskdop->height()-this->height())/3);
    Ram_warning_act=new DoublePortRam;
    timer_actuator=new QTimer(this);
    timer_actuator->start(1000);
    connect(timer_actuator, SIGNAL(timeout()),this,SLOT(warning_act_slot()));
    connect(ui->back_lb, SIGNAL(clicked()),this,SLOT(back_slot()));
    ui->plainTextEdit->setStyleSheet("QPlainTextEdit{font:bold;font-size:26px;color:red;background-color: rgb(255, 255, 255)}");

}

WarningActuator::~WarningActuator()
{
    delete ui;
}
void WarningActuator::warning_act_slot()
{

    char require_Act=Ram_warning_act->Receive(0x304);
    if(require_Act==0x55)
    {
        emit sendActuatorWarning(true);
       // char actuator_1_L=0xf0;
       // char actuator_1_H=0x0f;
      char actuator_1_L=Ram_warning_act->Receive(0x570);
       char actuator_1_H=Ram_warning_act->Receive(0x571);
        int actuator_1=(actuator_1_H<<8)|actuator_1_L;
        for(int i=0;i<16;i++)
        {
            int mark=(actuator_1>>i)&0x0001;
            if(mark==1)
                ui->plainTextEdit->insertPlainText(QString::fromUtf8("1 号 选 针 器 %1 刀 异 常 ！\n").arg(1+i));
        }

         char actuator_2_L=Ram_warning_act->Receive(0x572);
         char actuator_2_H=Ram_warning_act->Receive(0x573);
         int actuator_2=(actuator_2_H<<8)|actuator_2_L;
         for(int i=0;i<16;i++)
         {
             int mark=(actuator_2>>i)&0x0001;
             if(mark==1)

                 ui->plainTextEdit->insertPlainText(QString::fromUtf8("2 号 选 针 器 %1 刀  异 常 ！\n").arg(16-i));
         }
          char actuator_3_L=Ram_warning_act->Receive(0x574);
          char actuator_3_H=Ram_warning_act->Receive(0x575);
          int actuator_3=(actuator_3_H<<8)|actuator_3_L;
          for(int i=0;i<16;i++)
          {
              int mark=(actuator_3>>i)&0x0001;
              if(mark==1)

                  ui->plainTextEdit->insertPlainText(QString::fromUtf8("3 号 选 针 器 %1 刀 异 常 ！\n").arg(16-i));
          }
           char actuator_4_L=Ram_warning_act->Receive(0x576);
           char actuator_4_H=Ram_warning_act->Receive(0x577);
           int actuator_4=(actuator_4_H<<8)|actuator_4_L;
           for(int i=0;i<16;i++)
           {
               int mark=(actuator_4>>i)&0x0001;
               if(mark==1)

                   ui->plainTextEdit->insertPlainText(QString::fromUtf8("4 号 选 针 器 %1 刀 异 常 ！\n").arg(16-i));
           }
            char actuator_5_L=Ram_warning_act->Receive(0x578);
            char actuator_5_H=Ram_warning_act->Receive(0x579);
            int actuator_5=(actuator_5_H<<8)|actuator_5_L;
            for(int i=0;i<16;i++)
            {
                int mark=(actuator_5>>i)&0x0001;
                if(mark==1)

                    ui->plainTextEdit->insertPlainText(QString::fromUtf8("5 号 选 针 器 %1 刀 异 常 ！\n").arg(16-i));
            }
             char actuator_6_L=Ram_warning_act->Receive(0x57A);
             char actuator_6_H=Ram_warning_act->Receive(0x57B);
             int actuator_6=(actuator_6_H<<8)|actuator_6_L;
             for(int i=0;i<16;i++)
             {
                 int mark=(actuator_6>>i)&0x0001;
                 if(mark==1)

                     ui->plainTextEdit->insertPlainText(QString::fromUtf8("6 号 选 针 器 %1 刀 异 常 ！\n").arg(16-i));
             }
              char actuator_7_L=Ram_warning_act->Receive(0x57C);
              char actuator_7_H=Ram_warning_act->Receive(0x57D);
              int actuator_7=(actuator_7_H<<8)|actuator_7_L;
              for(int i=0;i<16;i++)
              {
                  int mark=(actuator_7>>i)&0x0001;
                  if(mark==1)

                      ui->plainTextEdit->insertPlainText(QString::fromUtf8("7 号 选 针 器 %1 刀 异 常 ！\n").arg(16-i));
              }
               char actuator_8_L=Ram_warning_act->Receive(0x57E);
               char actuator_8_H=Ram_warning_act->Receive(0x57F);
               int actuator_8=(actuator_8_H<<8)|actuator_8_L;
               for(int i=0;i<16;i++)
               {
                   int mark=(actuator_8>>i)&0x0001;
                   if(mark==1)

                       ui->plainTextEdit->insertPlainText(QString::fromUtf8("8 号 选 针 器 %1 刀 异 常 ！\n").arg(16-i));
               }
                char actuator_9_L=Ram_warning_act->Receive(0x580);
                char actuator_9_H=Ram_warning_act->Receive(0x581);
                int actuator_9=(actuator_9_H<<8)|actuator_9_L;
                for(int i=0;i<16;i++)
                {
                    int mark=(actuator_9>>i)&0x0001;
                    if(mark==1)

                        ui->plainTextEdit->insertPlainText(QString::fromUtf8("9 号 选 针 器 %1 刀 异 常 ！\n").arg(16-i));
                }
                 char actuator_10_L=Ram_warning_act->Receive(0x582);
                 char actuator_10_H=Ram_warning_act->Receive(0x583);
                 int actuator_10=(actuator_10_H<<8)|actuator_10_L;
                 for(int i=0;i<16;i++)
                 {
                     int mark=(actuator_10>>i)&0x0001;
                     if(mark==1)

                         ui->plainTextEdit->insertPlainText(QString::fromUtf8("10 号 选 针 器 %1 刀 异 常 ！\n").arg(16-i));
                 }
                  char actuator_11_L=Ram_warning_act->Receive(0x584);
                  char actuator_11_H=Ram_warning_act->Receive(0x585);
                  int actuator_11=(actuator_11_H<<8)|actuator_11_L;
                  for(int i=0;i<16;i++)
                  {
                      int mark=(actuator_11>>i)&0x0001;
                      if(mark==1)

                          ui->plainTextEdit->insertPlainText(QString::fromUtf8("11 号 选 针 器 %1 刀 异 常 ！\n").arg(16-i));
                  }
                   char actuator_12_L=Ram_warning_act->Receive(0x586);
                   char actuator_12_H=Ram_warning_act->Receive(0x587);
                   int actuator_12=(actuator_12_H<<8)|actuator_12_L;
                   for(int i=0;i<16;i++)
                   {
                       int mark=(actuator_12>>i)&0x0001;
                       if(mark==1)

                           ui->plainTextEdit->insertPlainText(QString::fromUtf8("12 号 选 针 器 %1 刀 异 常 ！\n").arg(16-i));
                   }

                    char actuator_13_L=Ram_warning_act->Receive(0x588);
                    char actuator_13_H=Ram_warning_act->Receive(0x589);
                    int actuator_13=(actuator_13_H<<8)|actuator_13_L;
                    for(int i=0;i<16;i++)
                    {
                        int mark=(actuator_13>>i)&0x0001;
                        if(mark==1)

                            ui->plainTextEdit->insertPlainText(QString::fromUtf8("13 号 选 针 器 %1 刀 异 常 ！\n").arg(16-i));
                    }
                     char actuator_14_L=Ram_warning_act->Receive(0x58A);
                     char actuator_14_H=Ram_warning_act->Receive(0x58B);
                     int actuator_14=(actuator_14_H<<8)|actuator_14_L;
                     for(int i=0;i<16;i++)
                     {
                         int mark=(actuator_14>>i)&0x0001;
                         if(mark==1)

                             ui->plainTextEdit->insertPlainText(QString::fromUtf8("14 号 选 针 器 %1 刀 异 常 ！\n").arg(16-i));
                     }

                      char actuator_15_L=Ram_warning_act->Receive(0x58C);
                      char actuator_15_H=Ram_warning_act->Receive(0x58D);
                      int actuator_15=(actuator_15_H<<8)|actuator_15_L;
                      for(int i=0;i<16;i++)
                      {
                          int mark=(actuator_15>>i)&0x0001;
                          if(mark==1)

                              ui->plainTextEdit->insertPlainText(QString::fromUtf8("15 号 选 针 器 %1 刀 异 常 ！\n").arg(16-i));
                      }
                       char actuator_16_L=Ram_warning_act->Receive(0x58E);
                       char actuator_16_H=Ram_warning_act->Receive(0x58F);
                       int actuator_16=(actuator_16_H<<8)|actuator_16_L;
                       for(int i=0;i<16;i++)
                       {
                           int mark=(actuator_16>>i)&0x0001;
                           if(mark==1)

                               ui->plainTextEdit->insertPlainText(QString::fromUtf8("16 号 选 针 器 %1 刀 异 常 ！\n").arg(16-i));
                       }
    }
    //报警消除
    char cancle_Act=Ram_warning_act->Receive(0x305);
    if(cancle_Act==0x55)
    {
         Ram_warning_act->Send(0xAA,0x305);
         Ram_warning_act->Send(0xAA,0x304);
         this->close();
    }
}
//fanhui
void WarningActuator::back_slot()
{
    timer_actuator->stop();/***/
    this->close();
}
