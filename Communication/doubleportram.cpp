#include "doubleportram.h"
/*驱动相关*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Communication/noanswer.h"

DoublePortRam::DoublePortRam()
{
     // pNoanswer=new Noanswer;
}
DoublePortRam::~DoublePortRam()
{
   // delete  pNoanswer;
}

/*发送函数*/
void DoublePortRam::Send(char DATA, long unsigned int ADD)
{
#ifdef Q_OS_LINUX
  ::write(DeviceSwitch ,&DATA, ADD);
#endif
}

/*接收函数*/
quint8 DoublePortRam::Receive( long unsigned int ADD)
{
#ifdef Q_OS_LINUX
   char DATA=0;
   ::read(DeviceSwitch ,&DATA, ADD);
   return (quint8)DATA;
#endif
}

//读取2字节参数
quint16 DoublePortRam::GetU16Data(unsigned int addr)
{
    quint16 data=0;
    char data_L=Receive(addr);
    char data_H=Receive(addr+1);
    data=(data_H<<8)|data_L;
    return data;
}

/*UI向RTC发送指令*/
bool DoublePortRam::UI_to_RTC()
{
    bool b=false;
    int count=0;
    while(!b)
    {
        //发送数据时，需要先先判断0x301地址处的值是否为0xAA（空闲），如果是则可以开始发送数据
        //数据发送完，需要在0x301处写0x55，表示数据发送完。
        char sendmark=Receive(0x301);//接收0x301数据
        if(sendmark!=0x55)//当不为0x55时说明处于空闲状态，可以发送指令
        {
            Send(SendData[0],0x100);//指令代码
            Send(SendData[1],0x101);//功能代码
            Send(SendData[2],0x102);//参数1
            Send(SendData[3],0x103);//参数2
            Send(SendData[4],0x104);//参数3
            Send(SendData[5],0x105);//参数4
            Send(SendData[6],0x106);//参数5
            Send(SendData[7],0x107);//参数6
            Send(SendData[8],0x108);//参数7
            Send(SendData[9],0x109);//参数8
            Send(0x55,0x301);
            b=true;
            break;
        }
        usleep(50);
        count++;
        if(count>=6000)
            break;
    }
    if(!b)
    {
        Noanswer *var=new Noanswer;
        var->exec();
        Send(0xAA,0x301);
    }
    return b;
}

/*RTC向UI发送指令*/
void DoublePortRam::RTC_to_UI()
{
    for(int i=0;i<2000;i++)
    {
       char interrupt=Receive(0x300);
        if(interrupt==0x55)
        {
            ReceiveData[0]=Receive(0x200);//指令代码
            ReceiveData[1]=Receive(0x201);//功能代码
            ReceiveData[2]=Receive(0x202);//参数1
            ReceiveData[3]=Receive(0x203);//参数2
            ReceiveData[4]=Receive(0x204);//参数3
            ReceiveData[5]=Receive(0x205);//参数4
            ReceiveData[6]=Receive(0x206);//参数5
            ReceiveData[7]=Receive(0x207);//参数6
            ReceiveData[8]=Receive(0x208);//参数7
            ReceiveData[9]=Receive(0x209);//参数8
            Send(0xAA,0x300);
            break;
        }
    }
}

/*切换到花型编织界面*/
void DoublePortRam::to_PatternWoven_slot()
{
    SendData[0]=0xA0;
    SendData[1]=0x20;
    SendData[2]=0x02;
    SendData[3]=0x00;
    SendData[4]=0x00;
    SendData[5]=0x00;
    SendData[6]=0x00;
    SendData[7]=0x00;
    SendData[8]=0x00;
    SendData[9]=0x00;
    UI_to_RTC();
}

void DoublePortRam::to_Menu()
{
    SendData[0]=0xA0;
    SendData[1]=0x20;
    SendData[2]=0x01;
    SendData[3]=0x00;
    SendData[4]=0x00;
    SendData[5]=0x00;
    SendData[6]=0x00;
    SendData[7]=0x00;
    SendData[8]=0x00;
    SendData[9]=0x00;
    UI_to_RTC();
}
/*切换到机器设置*/
void DoublePortRam::to_SettingMain_slot()
{
    SendData[0]=0xA0;
    SendData[1]=0x20;
    SendData[2]=0x04;
    SendData[3]=0x00;
    SendData[4]=0x00;
    SendData[5]=0x00;
    SendData[6]=0x00;
    SendData[7]=0x00;
    SendData[8]=0x00;
    SendData[9]=0x00;
    UI_to_RTC();
}
/*切换到系统维护*/
void DoublePortRam::to_SystemMaintance()
{
    SendData[0]=0xA0;
    SendData[1]=0x20;
    SendData[2]=0x08;
    SendData[3]=0x00;
    SendData[4]=0x00;
    SendData[5]=0x00;
    SendData[6]=0x00;
    SendData[7]=0x00;
    SendData[8]=0x00;
    SendData[9]=0x00;
    UI_to_RTC();
}
/*切换到机器测试界面*/
void DoublePortRam::to_TestingMain_slot()
{
    SendData[0]=0xA0;
    SendData[1]=0x20;
    SendData[2]=0x05;
    SendData[3]=0x00;
    SendData[4]=0x00;
    SendData[5]=0x00;
    SendData[6]=0x00;
    SendData[7]=0x00;
    SendData[8]=0x00;
    SendData[9]=0x00;
    UI_to_RTC();
}
/*切换到文件操作*/
void DoublePortRam::to_Fileoperations_slot()
{
    SendData[0]=0xA0;
    SendData[1]=0x20;
    SendData[2]=0x03;
    SendData[3]=0x00;
    SendData[4]=0x00;
    SendData[5]=0x00;
    SendData[6]=0x00;
    SendData[7]=0x00;
    SendData[8]=0x00;
    SendData[9]=0x00;
    UI_to_RTC();
}

