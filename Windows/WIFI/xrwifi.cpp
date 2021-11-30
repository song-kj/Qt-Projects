#include "xrwifi.h"
#include <QDateTime>
#include <QApplication>
#include <QSettings>
#include "Communication/doubleportram.h"
#include <QMessageBox>
#define MAC_ADDR_PATH "/xrfile/mac_addr.txt" //MAC地址文件路径
#define IOM_PATH "/xrfile/iom.txt"

extern DoublePortRam Ram;

XRWiFi::XRWiFi(QObject *parent)
{
    wifiConnectCount=0;
    wifiRebootCount=0;
    wifiStatusCount=0;
    baojingceshi=0;
    atreceiveCount=0;
    wifiType=WIFI_NONE;
    WiFiConnected=false;
    sendStaticPar=false;
    connectProgress=-1;
    exitTouChuanrequest=false;
    exitTouChuanCount=0;
    restart=false;
    timerInterval=3;
    restartCount=0;
    restsum=0;
    machineState=Stop;
    //定时器1，发送动态数据
    timer1=new QTimer(this);
    connect(timer1,SIGNAL(timeout()),this,SLOT(sendDynamicData()));
    timer3=new QTimer(this);
    connect(timer3,SIGNAL(timeout()),this,SLOT(WiFiStatus()));
    timer3->start(1000);
}

/*
 * 设置串口名和波特率，并打开串口
 * 返回true：串口打开成功；false：串口打开失败
 */
bool XRWiFi::setWiFiComPort(MyCom com)
{
    mycom_WiFi.setPortName(com.name);
    mycom_WiFi.setBaudRate(com.baudratetype);
    if(mycom_WiFi.open(QIODevice::ReadWrite))
    {
        //串口打开成功，从文件读取WiFi连接信息
        readFileOffWIFIPAR();
        connect(&mycom_WiFi,SIGNAL(readyRead()),this,SLOT(getData()));
        wifiType=WIFI_NONE;
        receive_at=false;
        //获得WiFi的Mac地址
        WiFiConnect(false);
        return true;
    }
    else
        return false;
}

QString XRWiFi::curTime()
{
    return QString::number(QDateTime::currentDateTime().toTime_t()-28800,16);
}

void XRWiFi::closeWiFiComPort()
{
    mycom_WiFi.close();
}

//设置WiFi连接参数
void XRWiFi::setWiFiParameter(WiFiParameter par)
{
    my_WiFi=par;
}

//发送数据
void XRWiFi::sendData(QByteArray data)
{
    sendBuffer=data;
    sendWiFiData(sendBuffer,0);
    mycom_WiFi.write(data.data());
    sendBuffer.clear();
}

//从文件读取WiFi配置信息
void XRWiFi::readFileOffWIFIPAR()
{
    /**读取文件中的数据**/
    QString machId,wifiName,wifiPassword,severIP,severPort,autoCnt,remote;
    QSettings *set=new QSettings(IOM_PATH,QSettings::IniFormat);
    machId= set->value("WiFiParameter/MachineID","").toString();
    wifiName= set->value("WiFiParameter/WiFiName","").toString();
    wifiPassword= set->value("WiFiParameter/WiFiPassword","").toString();
    severIP= set->value("WiFiParameter/SeverIP","").toString();
    severPort= set->value("WiFiParameter/SeverPort","").toString();
    autoCnt=set->value("WiFiParameter/AutoConnect","").toString();
    remote=set->value("WiFiParameter/Remote","").toString();
    delete set;
    my_WiFi.machineID=machId;
    my_WiFi.WiFi_IP=severIP;
    my_WiFi.WiFi_Name=wifiName;
    my_WiFi.WiFi_Password=wifiPassword;
    my_WiFi.WiFi_Port=severPort;
    if(autoCnt=="1")
        my_WiFi.auto_Connect=true;
    else
        my_WiFi.auto_Connect=false;

    if(remote=="1")
        my_WiFi.remote_Operation=true;
    else
        my_WiFi.remote_Operation=false;

    emit sendWiFiParameter(my_WiFi,-1);
}

//点击连接WiFi按钮
void XRWiFi::connentWiFi_clicked(WiFiParameter par)
{
    my_WiFi=par;
    wifiType=WIFI_CONNECT;
    WiFiConnect(false);
}

//机运行状态切换
void XRWiFi::machineStatusSwitch(Machine_Status state)
{
    if(mycom_WiFi.isOpen())
    {
        if(state!=On)
            machineState=state;
        if(!WiFiConnected||exitTouChuanrequest)
            return;
        QString str;
        switch (state) {
        case On://开机
            str=tr("40").append(curTime())
                    .append(my_WiFi.machineID).append("020000000C").append(macAdress).append("\n");
            sendBuffer=str.toLatin1();
            sendData(sendBuffer);
            lianjiCount++;
            emit sendQString("Status:On");
            break;
        case Off://关机
            str=tr("40").append(curTime())
                    .append(my_WiFi.machineID).append("010000000800000001").append("\n");
            sendBuffer=str.toLatin1();
            sendData(sendBuffer);
            emit sendQString("Status:Off");
            break;
        case Run://运行
            str=tr("40").append(curTime())
                    .append(my_WiFi.machineID).append("050000000800000001").append("\n");
            sendBuffer=str.toLatin1();
            sendData(sendBuffer);
            emit sendQString("Status:Run");
            break;
        case Stop://停车
            str=tr("40").append(curTime())
                    .append(my_WiFi.machineID).append("030000000800000001").append("\n");
            sendBuffer=str.toLatin1();
            sendData(sendBuffer);
            emit sendQString("Status:Stop");
            break;
        case Alarm://报警
            emit sendQString("Status:Alarm");
            break;
        case AlarmClear://报警消除
            str=tr("40").append(curTime()
                                ).append(my_WiFi.machineID).append("0400000006ffff02\n");
            sendBuffer=str.toLatin1();
            sendData(sendBuffer);
            emit sendQString("Status:AlarmClear");
            break;
        default:
            break;
        }
    }
}

//发送静态数据
void XRWiFi::sendStaticData()
{
    QFile file("/xrfile/Parameter.er");
    file.open(QIODevice::ReadWrite);
    QDataStream in(&file);

    //每次连服务器需要发送这条静态数据指令
    //01
    QString ID=tr("0000").append(my_WiFi.machineID);//01

    QString Class("00020103");//02

    QString size("00000003");//03

    in.device()->seek(2);
    qint16 totalNeedles;//04
    in>>totalNeedles;
    QString total_needles=fourBytesDatas(QString::number(totalNeedles,16));

    QString total_numbers("00000000");//05

    in.device()->seek(10);
    qint16 selectors_qint16;//06
    in>>selectors_qint16;
    QString selectors=fourBytesDatas(QString::number(selectors_qint16,16));

    QString sel_space("00000000");//07

    in.device()->seek(4);
    qint16 interval_qint16;
    in>>interval_qint16;//08
    QString interval=fourBytesDatas(QString::number(interval_qint16,16));

    qint16 mechZero;
    in.device()->seek(6);
    in>>mechZero;//09
    QString mech_zero=fourBytesDatas(QString::number(mechZero,16));

    in.device()->seek(8);
    qint16 selZero;
    in>>selZero;//10
    QString sel_zero=fourBytesDatas(QString::number(selZero,16));

    quint16 totalPuls;
    in.device()->seek(12);
    in>>totalPuls;//11
    QString totle_puls=fourBytesDatas(QString::number(totalPuls,16));
    file.close();
    QString data_static=tr("30").append(curTime()).append(my_WiFi.machineID)
            .append("010000005C01").append("01").append(ID).append(Class).append(size).append(total_needles)
            .append(total_numbers).append(selectors).append(sel_space)
            .append(interval).append(mech_zero).append(sel_zero).append(totle_puls).append("\n");
    sendBuffer=data_static.toLatin1();
    sendData(sendBuffer);
    //staticDataSendOver=true;
}

//发送动态数据
void XRWiFi::sendDynamicData()
{
    if(machineState==Run&&WiFiConnected&&connectProgress==4&&!exitTouChuanrequest)
    {
        if(sendStaticPar)
        {
            sendStaticData();
            return;
        }
        QString cur_needles=fourBytesDatas(QString::number(dynamicData.needles,16));//当前针位

        QString cur_angle=fourBytesDatas(QString::number(dynamicData.angle,16));//当前角度

        QString cur_speed=fourBytesDatas(QString::number(dynamicData.speed,16));//当前速度

        QString sel_adjust=fourBytesDatas(QString::number(dynamicData.selAdjust,16));

        QString fix_needle=fourBytesDatas(QString::number(dynamicData.fixNeedle,16));

        QString dyn_speed=fourBytesDatas(QString::number(dynamicData.dynSpeed,16));

        QString lim_speed=fourBytesDatas(QString::number(dynamicData.limSpeed,16));

        QString max_speed=fourBytesDatas(QString::number(dynamicData.maxSpeed,16));

        QString run_speed=dynamicData.run_speed;

        QString inj_interval=fourBytesDatas(QString::number(dynamicData.injInterval,16));

        QString inj_numbers=fourBytesDatas(QString::number(dynamicData.injNumbers,16));

        QString deb_switch("00000000");

        QString deb_interval("00000000");

        QString deb_speed("00000000");

        QString op_state("00000000");

        QString ele_switch("00000000");

        QString up_lamp("00000000");

        QString down_lamp("00000000");

        QString flu_lamp("00000000");

        QString fan("00000000");

        QString fine_tuning1=fourBytesDatas(QString::number(dynamicData.fineTuning[0],16));

        QString fine_tuning2=fourBytesDatas(QString::number(dynamicData.fineTuning[1],16));

        QString fine_tuning3=fourBytesDatas(QString::number(dynamicData.fineTuning[2],16));

        QString fine_tuning4=fourBytesDatas(QString::number(dynamicData.fineTuning[3],16));

        QString fine_tuning5=fourBytesDatas(QString::number(dynamicData.fineTuning[4],16));

        QString fine_tuning6=fourBytesDatas(QString::number(dynamicData.fineTuning[5],16));

        QString fine_tuning7=fourBytesDatas(QString::number(dynamicData.fineTuning[6],16));

        QString fine_tuning8=fourBytesDatas(QString::number(dynamicData.fineTuning[7],16));

        QString cur_yield=fourBytesDatas(QString::number(dynamicData.cur_yield,16));

        QString set_yield=fourBytesDatas(QString::number(dynamicData.set_yield,16));

        QString total_yield=fourBytesDatas(QString::number(dynamicData.total_yield,16));

        QString single_time=fourBytesDatas(QString::number(dynamicData.single_time,16));
        //TIME = single_time;
        QString duty_id=dynamicData.duty_id;

        //04
        QString pro_name=twentyBytesData(dynamicData.pro_name);
        QString total_step=fourBytesDatas(QString::number(dynamicData.total_step,16));
        QString cur_step=fourBytesDatas(QString::number(dynamicData.cur_step,16));
        QString total_circle=fourBytesDatas(QString::number(dynamicData.total_circle,16));
        QString cur_circle=fourBytesDatas(QString::number(dynamicData.cur_circle,16));

        QString data_dynamic=tr("30").append(curTime())
                .append(my_WiFi.machineID).append("010000015801")
                .append("02").append(cur_needles).append(cur_angle)
                .append(cur_speed).append(sel_adjust).append(fix_needle)
                .append(dyn_speed).append(lim_speed)
                .append(max_speed).append(run_speed)
                .append(inj_interval).append(inj_numbers)
                .append(deb_switch).append(deb_interval)
                .append(deb_speed).append(op_state)
                .append(ele_switch).append(up_lamp).append(down_lamp)
                .append(flu_lamp).append(fan).append(fine_tuning1)
                .append(fine_tuning2).append(fine_tuning3)
                .append(fine_tuning4).append(fine_tuning5)
                .append(fine_tuning6).append(fine_tuning7)
                .append(fine_tuning8).append("03").append(cur_yield)
                .append(set_yield).append(total_yield)
                .append(single_time).append(duty_id)
                .append("04").append(pro_name)
                .append(total_step).append(cur_step).append(total_circle)
                .append(cur_circle).append("\n");
        //实时上报数据
       sendBuffer=data_dynamic.toLatin1();
       sendData(sendBuffer);

    }
}

//更新系统时间
void XRWiFi::update_time(const QString &s_time)
{
    bool ok;
    int sum_sec = s_time.toInt(&ok,16)+28800;
#ifdef Q_OS_LINUX
    struct tm nowtime;
    time_t t;
    nowtime.tm_sec = QDateTime::fromTime_t(sum_sec).time().second();//ui->second_btn->text().toInt();/* Seconds.[0-60](1 leap second)*/
    nowtime.tm_min = QDateTime::fromTime_t(sum_sec).time().minute();//ui->minute_btn->text().toInt();/* Minutes.[0-59] */
    nowtime.tm_hour = QDateTime::fromTime_t(sum_sec).time().hour();//ui->hour_btn->text().toInt();/* Hours. [0-23] */
    nowtime.tm_mday = QDateTime::fromTime_t(sum_sec).date().day();//ui->day_btn->text().toInt();/*  Day.[1-31]  */
    nowtime.tm_mon= QDateTime::fromTime_t(sum_sec).date().month()-1;//ui->month_btn->text().toInt()-1;/* Month.[0-11]*/
    nowtime.tm_year= QDateTime::fromTime_t(sum_sec).date().year()-1900;//ui->year_btn->text().toInt()-1900;/* Year- 1900.*/
    nowtime.tm_isdst=-1;/*DST.[-1/0/1]*/
    t=mktime(&nowtime);
    stime(&t);
    system("hwclock -w");
#endif
}

//远程锁机
void XRWiFi::LockDevice(QString passwd)
{
    if(!my_WiFi.global_locked)
    {
        /*指令，提示下位机到期*/
        Ram.SendData[0] = 0x20;
        Ram.SendData[1] = 0xA4;
        Ram.SendData[2] = 0x55;
        Ram.SendData[3] = 0x00;
        Ram.SendData[4] = 0x00;
        Ram.SendData[5] = 0x00;
        Ram.SendData[6] = 0x00;
        Ram.SendData[7] = 0x00;
        Ram.SendData[8] = 0x00;
        Ram.SendData[9] = 0x00;
        Ram.UI_to_RTC();

        my_WiFi.global_locked = true;
        //写入锁机成功和锁机密码
        QSettings *set=new QSettings(IOM_PATH,QSettings::IniFormat);
        set->setValue("WiFiParameter/Locked",1);
        set->setValue("WiFiParameter/LockPassWD",passwd);
        delete set;
        system("sync");
        QString str=tr("10").append(curTime()).append(my_WiFi.machineID).append(
                    "030000000800000001").append("\n");//
        sendBuffer=str.toLatin1();
        sendData(sendBuffer);

        emit sendQString("Msg:"+tr("设备远程锁定成功！"));
    }
}

//远程解锁
void XRWiFi::UnlockDevice(QString passwd)
{
    //已锁定
    if(my_WiFi.global_locked)
    {
        QString raw_passwrd;
        QSettings *set=new QSettings(IOM_PATH,QSettings::IniFormat);
        //读取本地锁机密码
        raw_passwrd= set->value("WiFiParameter/LockPassWD","").toString();
        //读取机号
        quint8 idInt[10];
        QFile fileId(tr("/xrfile/").append("id.txt"));
        if(fileId.open(QIODevice::ReadWrite))
        {
            QString str;
            QTextStream out(&fileId);
            out>>str;
            fileId.close();
            for(int i=0;i<10;i++)
            {
                idInt[i] = str.left(1).toInt();
                str = str.remove(0,1);
            }
        }
        //判断密码是否正确
        if(passwd==raw_passwrd)
        {
            for(int i=0;i<200000;i++)
            {
                char sendmark=Ram.Receive(0x301);//接收0x301数据
                if(sendmark!=0x55)//当不为0x55时说明处于空闲状态，可以发送指令
                {
                    Ram.Send(0xA0,0x100);//指令代码
                    Ram.Send(0x20,0x101);//功能代码
                    Ram.Send(0x07,0x102);//参数1
                    Ram.Send(idInt[0],0x103);//参数2
                    Ram.Send(idInt[1],0x104);//参数3
                    Ram.Send(idInt[2],0x105);//参数4
                    Ram.Send(idInt[3],0x106);//参数5
                    Ram.Send(idInt[4],0x107);//参数6
                    Ram.Send(idInt[5],0x108);//参数7
                    Ram.Send(idInt[6],0x109);//参数8
                    Ram.Send(idInt[7],0x10a);//参数9
                    Ram.Send(idInt[8],0x10b);//参数10
                    Ram.Send(idInt[9],0x10c);//参数8resetBool=false;
                    Ram.Send(0x55,0x301);
                    my_WiFi.global_locked = false;//解锁成功
                    break;
                }
                if(i==200000-1)
                {
                    my_WiFi.global_locked = true;//解锁失败
                }
            }

            //写入锁定成功
            set->setValue("WiFiParameter/Locked",my_WiFi.global_locked==true?1:0);
            system("sync");

            if(!my_WiFi.global_locked)
            {
                QString str=tr("10").append(curTime()).append(my_WiFi.machineID).append(
                            "040000000800000001").append("\n");//
                sendBuffer=str.toLatin1();
                sendData(sendBuffer);
                emit sendQString("Msg:"+tr("设备远程解锁成功！"));
            }
            else
            {
                QString str=tr("10").append(curTime()).append(my_WiFi.machineID).append(
                            "040000000800000000").append("\n");//
                sendBuffer=str.toLatin1();
                sendData(sendBuffer);
                emit sendQString("Msg:"+tr("设备远程解锁失败！"));
            }
        }
        else//密码错误，解锁失败
        {
            my_WiFi.global_locked = true;
            QString str=tr("10").append(curTime()).append(my_WiFi.machineID).append(
                        "040000000800000000").append("\n");//
            sendBuffer=str.toLatin1();
            sendData(sendBuffer);
            emit sendQString("Msg:"+tr("密码错误，设备远程解锁失败！"));
        }
    }
}

//产量设定
void XRWiFi::setYield()
{
    if(my_WiFi.remote_Operation)
    {
        QString value  = receiveData.right(12);
        QString mark1 = value.left(2);
        value.remove(0,2);
        //目前只允许修改总产量值
        if(mark1=="03"&&value.left(2)=="02")
        {
            if(Ram.Receive(0x100)==0x20)
            {
                bool ok;
                int intvalue = value.right(8).toInt(&ok,16);
                if(intvalue<=dynamicData.cur_yield)  //如果设定值小于或等于当前产量，则不允许修改
                {
                    QString str=tr("10").append(curTime()).append(my_WiFi.machineID)
                            .append("020000000800000011").append("\n");
                    sendBuffer=str.toLatin1();
                    sendData(sendBuffer);
                    emit sendQString("Msg:"+tr("总产量设定值不能小于当前产量！请重新设置"));
                }
                else
                {
                    emit sendQString("Total_yield:"+QString::number(intvalue));
                    QFile file("/xrfile/Parameter.er");
                    if(file.open(QIODevice::ReadWrite))
                    {
                        QDataStream out(&file);
                        out.device()->seek(26);
                        out<<(quint16)intvalue;
                        file.flush();
                        system("sync");
                        file.close();
                    }
                    Ram.SendData[0]=0x20;
                    Ram.SendData[1]=0xA2;
                    Ram.SendData[2]=0x06;//产量设置
                    Ram.SendData[3]=intvalue%256;
                    Ram.SendData[4]=intvalue/256;
                    Ram.SendData[5]=dynamicData.cur_yield%256;
                    Ram.SendData[6]=dynamicData.cur_yield/256;
                    Ram.SendData[7]=0;
                    Ram.SendData[8]=0;
                    Ram.SendData[9]=0;
                    Ram.UI_to_RTC();

                    for(int ii=0;ii<3000;ii++)
                    {

                    }
                    Ram.to_PatternWoven_slot();//切换到花型编织界面
                    for(int i=0;i<3000;i++)
                    {

                    }
                    //切换到花型编制界面后再进入正常编织
                    Ram.SendData[0]=0x20;
                    Ram.SendData[1]=0xA1;
                    Ram.SendData[2]=0x00;
                    Ram.SendData[3]=0x00;
                    Ram.SendData[4]=0x00;
                    Ram.SendData[5]=0x00;
                    Ram.SendData[6]=0x00;
                    Ram.SendData[7]=0x00;
                    Ram.SendData[8]=0x00;
                    Ram.SendData[9]=0x00;
                    Ram.UI_to_RTC();
                    QString str=tr("10").append(curTime()).append(my_WiFi.machineID)
                            .append("020000000800000001").append("\n");
                    sendBuffer=str.toLatin1();
                    sendData(sendBuffer);
                    emit sendQString("Msg:"+tr("总任务量修改成功！修改值为:%1").arg(QString::number(intvalue)));
                }
            }
            else
            {
                QString str=tr("10").append(curTime()).append(my_WiFi.machineID)
                        .append("020000000800000011").append("\n");
                sendBuffer=str.toLatin1();
                sendData(sendBuffer);
                emit sendQString("Msg:"+tr("此编织界面下不允许修改总产量值！"));

            }
        }
    }
    else
    {
        QString str=tr("10").append(curTime()).append(my_WiFi.machineID)
                .append("020000000800000000").append("\n");
        sendBuffer=str.toLatin1();
        sendData(sendBuffer);
        }
}

void XRWiFi::restartWiFi()
{
    sendBuffer="---";//掉电重启模块
    restsum++;
    wifiStatusCount=0;
    atreceiveCount=0;
    restartCount=0;
    WiFiConnected=false;
    exitTouChuanrequest=false;
    connectProgress=0;
    restart=true;
    sendData(sendBuffer);
    emit sendQString("DISCONNECTED");
}

void XRWiFi::sendATinstruction(QString str)
{
    sendBuffer=str.toLatin1();
    sendData(sendBuffer);
    receive_at=false;
}

//检测网络状态
void XRWiFi::WiFiStatus()
{
    if(mycom_WiFi.isOpen())
    {
        if(connectProgress==4)
        {
            if(machineState==Run)
            {
                wifiStatusCount++;
                restartCount++;
            }
        }
        else
        {
            wifiStatusCount++;
            restartCount++;
        }
        if(wifiStatusCount%10==0)
        {
            //发送AT指令未收到回复  继续发送
            if(!receive_at)
            {
                //5次未收到AT指令回复 重启模块
                if(atreceiveCount>=5)
                {
                    receive_at=true;
                    restartWiFi();
                    return;
                }
                if(exitTouChuanrequest)//退出透传后发AT指令不回复  继续执行closeWiFi
                {
                    exitTouChuanrequest=false;
                    closeWiFi();
                }
                else
                    WiFiConnect(false);
                atreceiveCount++;

            }
            //联机指令未收到回复，继续发送联机指令
            if(wifiType==TCP_SEND&&connectProgress==3)
                WiFiConnect(false);
        }
        if(restart)
        {
            //模块重启后从获取NAC地址开始执行一遍
            if(wifiStatusCount>15)
            {
                mycom_WiFi.readAll();
                restart=false;
                wifiStatusCount=0;
                wifiType=WIFI_NONE;
                WiFiConnect(false);
            }
        }
        if(restartCount>200&&my_WiFi.auto_Connect)
        {
            restartWiFi();
            return;
        }
        if(wifiStatusCount>timerInterval*5&&wifiScaned)
        {
            /*-------------------------*/
            if(WiFiConnected)//18s超时后 自动连接wifi
            {
                wifiStatusCount=0;
                wifiType=TCP_CLOSE;
                MySleep(2000);
                closeWiFi();
            }
            else {
                emit sendQString("DISCONNECTED");
                connectProgress=0;
                if(my_WiFi.auto_Connect)
                {
                    wifiStatusCount=0;
//                    connentWiFi_clicked(my_WiFi);
                    WiFiConnect(false);
                }
            }
        }
    }
    else {
        timer1->stop();
    }
}

//断开WiFi
void XRWiFi::closeWiFi()
{
    if(exitTouChuanrequest==false)
    {
        //透传退5次后 重启模块
        if(exitTouChuanCount>=5)
        {
            restartWiFi();
            return;
        }
        //退出透传模式
        exitTouChuanrequest=true;
        wifiStatusCount=1;
        MySleep(1000);
        sendBuffer="+++";
        qDebug()<<"+++";
        sendData(sendBuffer);
        exitTouChuanCount++;
        //延时1秒
        MySleep(1000);
        sendATinstruction("AT+CIPCLOSE\r\n");
    }
    else
    {

    }
}

/*向服务器写报警
 *num1：报警类型
 *num2：报警单元号
 */
void XRWiFi::writeAlarm(int num1, int num2)
{
    if(mycom_WiFi.isOpen()&&WiFiConnected&&!exitTouChuanrequest)
    {
        QString alarm=QString("%1").arg(num1,2,16,QLatin1Char('0'));
        QString alarm1=QString("%1").arg(num2,2,16,QLatin1Char('0'));
        QString wrn=tr("40").append(curTime()).append(my_WiFi.machineID).append(
                    "0400000006").append(alarm).append(alarm1).append("01\n");
        //写入报警数据
        sendData(wrn.toLatin1());
        //切换报警状态
        machineStatusSwitch(Alarm);
    }
}

QString XRWiFi::fourBytesDatas(QString str)
{
    if(str.length()<9)
    {
        QString fourBytes("00000000");
        fourBytes.chop(str.length());
        return fourBytes.append(str);
    }
    else
    {
        return  str.remove(0,8);
    }
}

QString XRWiFi::twentyBytesData(QString ch)
{
    QString twentyBytes("0000000000000000000000000000000000000000");
    twentyBytes.chop(ch.length());
    return twentyBytes.append(ch);
}

//延时函数 ms
void XRWiFi::MySleep(unsigned int msec)
{
    QTime reachTime=QTime::currentTime().addMSecs(msec);
        while(QTime::currentTime()<reachTime)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents,100);
        }
}

//读取串口数据
void XRWiFi::getData()
{
    if(mycom_WiFi.bytesAvailable()>=1)
    {
        wifiStatusCount=0;
        receiveData+=mycom_WiFi.readAll();
        if(receiveData.contains("ready"))
        {
            atreceiveCount=0;
            connectProgress=0;
            emit sendWiFiData(receiveData,1);
            receiveData.clear();
            WiFiConnected=false;
            return;
        }
        if (exitTouChuanrequest) {
            receive_at=true;
            atreceiveCount=0;
            //收到OK或者ERROR表示透传已退出
            if(receiveData.contains("OK\r")||receiveData.contains("ERROR\r"))
            {
                exitTouChuanrequest=false;
                exitTouChuanCount=0;
                WiFiConnected=false;
                qDebug()<<"closed";
                emit sendWiFiData(receiveData,1);
                receiveData.clear();
                wifiType=TCP_CLOSE;
                sendATinstruction("AT+CWQAP\r\n");
                return;
            }
            else if(receiveData.contains("error\n"))//收到error 未退出透传
            {
                //继续退透传
                exitTouChuanrequest=false;
                emit sendWiFiData(receiveData,1);
                receiveData.clear();
                closeWiFi();
                return;
            }
            return;
        }
        //处理WiFi连接前的业务逻辑
        if(!WiFiConnected)
        {
            if(receiveData.contains("OK\r")||receiveData.contains("FAIL\r")||receiveData.contains("ERROR\r")
                    ||receiveData.contains(">")||receiveData.contains("+++"))
            {
                emit sendWiFiData(receiveData,1);
                receive_at=true;
                atreceiveCount=0;
                //获得Mac地址，设置STA模式并连接WiFi
                if(wifiType==WIFI_NONE||wifiType==WIFI_MAC||wifiType==WIFI_MODE||wifiType==WIFI_CONNECTED||wifiType==TOUCHUAN_SETTING)
                {
                    if(receiveData.contains("OK\r"))
                    {
                        WiFiConnect(true);
                    }
                    else
                    {
                        wifiConnectCount++;
                        if(wifiConnectCount<5)
                        {
                            WiFiConnect(false);
                        }
                        else {
                            wifiConnectCount=0;
                            wifiType=WIFI_DISCONNECT;
                            emit sendQString("DISCONNECTED");
                        }
                    }
                }
                else if(wifiType==WIFI_CONNECT)
                {
                    if(receiveData.contains("+CWLAP")==false)
                    {
                        if(receiveData.contains("OK\r"))
                        {
                            WiFiConnect(true);
                        }
                        else
                        {
                            wifiConnectCount++;
                            if(wifiConnectCount<5)
                            {
                                WiFiConnect(false);
                            }
                            else {
                                wifiConnectCount=0;
                                wifiType=WIFI_DISCONNECT;
                                emit sendQString("DISCONNECTED");
                            }
                        }
                    }
//                    else
//                        qDebug()<<"++++++++++++++++++++++++++++++\n"<<receiveData;
                }
                //服务器正常通讯后发送开机指令
                else if (wifiType==TCP_CONNECTED)
                {
                    //连接服务器成功，建立与服务器通讯，发送联机指令
                    if(receiveData.contains(">"))
                    {
                        connectProgress=3;
                        lianjiCount=0;
                        WiFiConnect(true);
                    }
                }
                else if (wifiType==WIFI_CLOSE||wifiType==TCP_CLOSE) {
                    emit sendQString("DISCONNECTED");
                    connectProgress=0;
                    wifiType=WIFI_NONE;
                   // timer3->stop();
                    if(my_WiFi.auto_Connect)
                    {
                        WiFiConnect(false);
                    }
                }
                else
                {
                    //emit sendWiFiData(receiveData);
                }
//                if(receiveData.contains("CLOSED\n"))//退出透传模式
//                {
//                    wifiType=TCP_CLOSE;
//                    closeWiFi();
//                }

                receiveData.clear();
            }
        }
        //处理WiFi连接后的业务逻辑
        else
        {
            if(receiveData.contains("\n")/*||receiveData.contains("\r")*/)
            {
                emit sendWiFiData(receiveData,1);
                if(receiveData.left(2).contains("1f"))
                {
                    restartCount=0;
                    if(receiveData.mid(10,4)==my_WiFi.machineID)
                    {
                        receiveData.chop(1);
                        if(receiveData.right(18).left(2) == "01")//开机设定频率
                        {
                            QString s_time = QString(receiveData.mid(2,8));
                            update_time(s_time);//时间校准
                            connectProgress=4;
                            lianjiCount=0;
                            timerInterval = receiveData.right(8).toInt();//动态数据发送间隔
//                            machineStatusSwitch(machineState);
                            sendStaticData();//开机时发送静态数据
                            sendStaticPar=true;
                            timer1->start(timerInterval*1000);//启动定时器1，发送动态数据
                        }
                        else if(receiveData.right(22).left(2)=="02")//服务器修改设备参数
                        {
                            //目前只允许总产量设定
                            setYield();
                        }
                        else if(receiveData.right(18).left(2) == "03")//远程锁机
                        {
                            if(my_WiFi.remote_Operation)//支持远程操作
                            {
                                QString passwrd = QString(receiveData.right(8));
                                LockDevice(passwrd);
                            }
                            else//不支持远程锁机
                            {
                                QString str=tr("10").append(curTime()).append(my_WiFi.machineID).append(
                                            "030000000800000000").append("\n");//
                                sendBuffer=str.toLatin1();
                                sendData(sendBuffer);
                            }
                        }
                        else if(receiveData.right(18).left(2) == "04")//远程解锁
                        {
                            if(my_WiFi.remote_Operation)//支持远程操作
                            {
                                QString passwrd = QString(receiveData.right(8));
                                UnlockDevice(passwrd);
                            }
                            else//不支持远程解锁
                            {
                                QString str=tr("10").append(curTime()).append(my_WiFi.machineID).append(
                                            "040000000800000000").append("\n");//
                                sendBuffer=str.toLatin1();
                                sendData(sendBuffer);
                            }
                        }
                    }
                }
                else if(receiveData.left(2).contains("2f"))
                {
                    restartCount=0;
                }
                else if(receiveData.left(2).contains("3f"))
                {
                    restartCount=0;
                    if(receiveData.mid(10,4)==my_WiFi.machineID)
                    {
                        if(sendStaticPar)
                        {
                            sendStaticPar=false;
                            if(machineState!=Run)
                                machineStatusSwitch(machineState);
                        }
                    }
                }
                else if(receiveData.left(2).contains("4f"))
                {
                    restartCount=0;
                }
                else if (receiveData.contains("ffff"))//心跳检测
                {
                    if(connectProgress==4)
                        restartCount=0;
                    sendBuffer="eeee\n";
                    sendData(sendBuffer);
                }
                else {
//                    QList<QByteArray> lb= receiveData.split(',');
//                    int dbm=lb[lb.length()-1].toInt();
//                    QString d="0";
//                    if(dbm>=-55)
//                    {
//                        d="4";
//                    }
//                    else if (dbm>=-70&&dbm<-55) {
//                        d="3";
//                    }
//                    else if (dbm>=-85&&dbm<-70) {
//                        d="2";
//                    }
//                    else if (dbm>=-100&&dbm<-85) {
//                        d="1";
//                    }
//                    else {
//                        d="0";
//                    }
//                    emit sendQString("dbm:"+d);
                }

                receiveData.clear();
            }
        }
    }
}

//开机获得Mac地址、设置STATION模式、扫描热点
void XRWiFi::WiFiConnect(bool request)
{
    if(request)
    {
        int wt= wifiType;
        wifiType=WIFI_typedef(wt+1);
    }
    if(wifiType==WIFI_NONE)
    {
        wifiScaned=false;
        //获取ESP8266 STA的MAC地址
        sendATinstruction("AT+CIPSTAMAC?\r\n");
    }
    else if(wifiType==WIFI_MAC)//写入MAC地址
    {
        int start=receiveData.indexOf('"');
        int end=receiveData.indexOf('"',start+1);
        macAdress=QString(receiveData.mid(start+1,end-start-1));
        macAdress=macAdress.remove(':');
        if(macAdress.length()==12)
        {
            QFile mac_file(QString(MAC_ADDR_PATH));
            if(mac_file.open(QIODevice::WriteOnly))
            {
                mac_file.resize(0);
                QTextStream mac(&mac_file);
                mac<<macAdress;
                mac_file.flush();
                mac_file.close();
                system("sync");
            }
        }
        //设置STATION模式
        sendATinstruction("AT+CWMODE=1\r\n");
    }
    else if(wifiType==WIFI_MODE)//扫描WiFi热点
    {
        //扫描热点
        sendATinstruction("AT+CWLAP\r\n");
    }
    else if (wifiType==WIFI_SCAN) {
        if(wifiScaned)
        {

        }
        else
        {
            wifiScaned=true;
            wifiType=WIFI_CONNECT;
            qDebug()<<"1111111111111111111111111"<<my_WiFi.auto_Connect;
            WiFiScan();
            if(my_WiFi.auto_Connect)
            {
                WiFiConnect(false);
            }
        }
    }
    else if (wifiType==WIFI_CONNECT)//连接WiFi
    {
        QString str;
        str=tr("AT+CWJAP=").append("\"").append(my_WiFi.WiFi_Name).append(
                    "\"").append(",").append("\"").append(
                    my_WiFi.WiFi_Password).append(
                    "\"").append("\r\n");
        sendATinstruction(str);
        emit sendQString("CONNECTING");
    }
    else if (wifiType==WIFI_CONNECTED) //WiFi连接成功，设置透传模式
    {
        connectProgress=1;
        sendATinstruction("AT+CIPMODE=1\r\n");
    }
    else if(wifiType==TOUCHUAN_SETTING)//透传设置成功，开始连接服务器
    {
        QString str;
        str=tr("AT+CIPSTART=").append("\"").append("TCP").append("\"").append(",").append("\"")
                .append(my_WiFi.WiFi_IP).append("\"").append(",").append(my_WiFi.WiFi_Port).append("\r\n");
        sendATinstruction(str);
    }
    else if(wifiType==TCP_CONNECTED)//开始透传
    {
        connectProgress=2;
        sendATinstruction("AT+CIPSEND\r\n");
    }
    else if(wifiType==TCP_SEND)//发送联机指令
    {
        if(lianjiCount>5)
        {
            closeWiFi();
            return;
        }
        WiFiConnected=true;
        emit sendQString("CONNECTED");
        machineStatusSwitch(On);
    }
}

//扫描当前可用WiFi热点
void XRWiFi::WiFiScan()
{
//    qDebug()<<"---------------------------------\n"<<receiveData;
    QStringList ql;
    QList<QByteArray> list = receiveData.split('+');
    if(list.length()>=2)
    {
        QList<QByteArray> listWifiName[list.size()-2];
        for(int i = 0;i <list.size();i++)
        {
            if(i > 1)
            {
                listWifiName[i-2] = list[i].split(',');
            }
        }
        for(int j = 0;j < list.size()-2;j++)
        {
            for(int k = 0;k < listWifiName[j].size();k++)
            {
                if(listWifiName[j].length()>2)
                {
                    QString str  = QString(listWifiName[j][1]);
                    str.replace("\"","");
                    ql<<str;
                    break;
                }
            }
        }
        int index=ql.indexOf(my_WiFi.WiFi_Name);
        emit sendQStringList(ql);
        emit sendWiFiParameter(my_WiFi,index);
    }
}
