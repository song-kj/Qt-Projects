#include "wifimode.h"
#include "ui_wifimode.h"
#include <QMessageBox>
#include <QDateTime>
#include <unistd.h>
#include <QFile>
#include <QTextStream>
#include "Communication/doubleportram.h"
#include <QDebug>
#include "Tool/KeyBoard/softwear.h"
#include <QDateTime>
#include <QDir>
#include "mymessagebox.h"
#include "myheader.h"

#define IOM_PATH "/xrfile/iom.txt"

extern QByteArray temp;
extern int keyMark;
extern quint16 sumOutP;
uint  global_NewMount;
bool global_FileToServer;
QString machineID;
bool global_bUnlocked;

const static quint32 Crc32Table[] =
{
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
    0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
    0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
    0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
    0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
    0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
    0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
    0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
    0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
    0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
    0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
    0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
    0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
    0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
    0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
    0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
    0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
    0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
    0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
    0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
    0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
    0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

/*
 * 连接wifi,服务器，发送数据的步骤：
 * 1、读MAC地址，AT+CIPSTAMAC?
 * 2、搜索当前可用的热点：AT+CWLAP
 * 3、设置STA模式：AT+CWMODE=1
 * 4、连接WIFI: AT+CWJAP="WIFI_name", "password",这一命令发出去后，会立刻受到一个WIFI DISCONNECTED 的字符串，不用急，等一会会有WIFI CONNECTED 的字符串，连上网络是需要一定的时间的。
 * 5、设置透传模式：AT+CIPMODE=1
 * 5、连接服务器：AT+CIPSTART="TCP", "112.16.93.229",8282
 * 6、然后发送AT+CIPSEND开始模式。进入透传模式成功，会返回'>'符号
 * 7、向服务器发连接状态指令
*/
WIFIMode::WIFIMode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WIFIMode)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    pQTimer=new QTimer(this);
    connect(pQTimer,SIGNAL(timeout()),this,SLOT(update_slot()));//处理键盘的输入
    pQTimer->start(10);

    myWiFi=new XRWiFi;
    connect(myWiFi,SIGNAL(sendWiFiData(QByteArray,int)),this,SLOT(getData(QByteArray,int)));
    connect(myWiFi,SIGNAL(sendWiFiParameter(WiFiParameter,int)),this,SLOT(getWiFiParameter(WiFiParameter,int)));
    connect(myWiFi,SIGNAL(sendQStringList(QStringList)),this,SLOT(getWiFiName(QStringList)));
    connect(myWiFi,SIGNAL(sendQString(QString)),this,SLOT(getWiFiString(QString)));
#ifdef Q_OS_UNIX
    myCom.name="ttyO5";
    myCom.baudratetype=BAUD115200;
#elif (defined Q_OS_WIN)
    myCom.name="COM3";
    myCom.baudratetype=BAUD115200;
#endif

    ui->autoConn_btn->setStyleSheet("QPushButton{border-style:outset;background: transparent;font-size:24px;color: rgb(255, 255, 255);}");
    ui->back_pb->setStyleSheet("QPushButton{border-radius:10;font-size:22px;color:white;background-color: rgb(100, 100, 100)}");

    if(myWiFi->setWiFiComPort(myCom))
    {
        comIsOpend=true;
    }
    else {
        comIsOpend=false;
//        QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("USB open failed!"),QMessageBox::Yes);
    }

    connect(ui->MachID_lb,SIGNAL(clicked()),this,SLOT(machIdClickSlot()));

    connect(ui->WIFIPassword_le,SIGNAL(clicked()),this,SLOT(wifiPwdClickSlot()));

    connect(ui->IPAdress_le,SIGNAL(clicked()),this,SLOT(tcpAdressClickSlot()));

    connect(ui->port_le,SIGNAL(clicked()),this,SLOT(tcpPortClickSlot()));

    connect(ui->textBrowser,SIGNAL(cursorPositionChanged()),this,SLOT(autoScroll()));

    ui->textBrowser->setVisible(false);
    //提示框
    m_Info = new ErrorCode;
}

WIFIMode::~WIFIMode()
{
    delete ui;
}

/**退出该界面**/
void WIFIMode::on_back_pb_clicked()
{
    emit update_MachineID();
    QSettings *set=new QSettings(IOM_PATH,QSettings::IniFormat);
    set->setValue("WiFiParameter/MachineID",ui->MachID_lb->text());
    delete set;
    system("sync");
    setVisible(false);
}

/**处理键盘输入**/
void WIFIMode::update_slot()
{
    if(this->isActiveWindow()&&temp.length()>=1)
    {
        if(temp[0]==(char)33)
        {
            digital_mark=0;
            temp[0]=255;
            if(wheatherOpenSettingWifiName)// wifi name bei xuan zhong
            {
                int count = ui->lWIFIName_le->count();

                if((wifiNameIndex < count)&&(wifiNameIndex >0))
                {
                    wifiNameIndex --;
                    ui->lWIFIName_le->view()->selectionModel()->
                            setCurrentIndex(ui->lWIFIName_le->model()->
                            index(wifiNameIndex,ui->lWIFIName_le->
                            modelColumn(),ui->lWIFIName_le->
                            rootModelIndex()),QItemSelectionModel::ClearAndSelect);
                }
            }
            else
            {
                if((wifiSettingGroupItemsSelect<5)&&(wifiSettingGroupItemsSelect>0))
                {//1 2 3 4
                    wifiSettingGroupItemsSelect--;
                    switch (wifiSettingGroupItemsSelect)
                    {
                    case 0:
                        wheatherSettingWifiName = false;
                        number = ui->MachID_lb->text();
                        settingStyle();
                        ui->MachID_lb->setStyleSheet("border:1px solid green;"
                                                     "font: 18pt \"宋体\";"

                                                     "border-color: rgb(255, 0, 0);");

                        break;
                    case 1:
                        wheatherSettingWifiName = true;/*如果选中设置wifi名称，就可以下拉*/
                        number = ui->lWIFIName_le->currentText();
                        settingStyle();
                        ui->lWIFIName_le->setStyleSheet("border:1px solid green;"
                                                        "font: 18pt \"宋体\";"

                                                        "border-color: rgb(255, 0, 0);");

                        break;
                    case 2:
                        wheatherSettingWifiName = false;
                        number = ui->WIFIPassword_le->text();
                        settingStyle();
                        ui->WIFIPassword_le->setStyleSheet("border:1px solid green;"
                                                           "font: 18pt \"宋体\";"

                                                           "border-color: rgb(255, 0, 0);");


                        break;
                    case 3:
                        wheatherSettingWifiName = false;
                        number = ui->IPAdress_le->text();
                        settingStyle();
                        ui->IPAdress_le->setStyleSheet("border:1px solid green;"
                                                       "font: 18pt \"宋体\";"

                                                       "border-color: rgb(255, 0, 0);");
                        break;
                    default:
                        break;
                    }
                }
            }
        }
        else if(temp[0]==(char)35)
        {
            temp[0]=255;
            digital_mark=0;
            if(wheatherOpenSettingWifiName)// wifi name bei xuan zhong
            {
                int count  = ui->lWIFIName_le->count();

              //  wifiNameIndex = ui->lWIFIName_le->currentIndex();

                if((wifiNameIndex < count-1)&&(wifiNameIndex >-1))
                {
                    wifiNameIndex ++;
                    ui->lWIFIName_le->view()->selectionModel()->setCurrentIndex(ui->lWIFIName_le->model()->index(wifiNameIndex,ui->lWIFIName_le->modelColumn(),ui->lWIFIName_le->rootModelIndex()),QItemSelectionModel::ClearAndSelect);
                }

            }
            else
            {
                if(wifiSettingGroupItemsSelect>-1&&wifiSettingGroupItemsSelect<4)
                {// 1 2 3 4
                    wifiSettingGroupItemsSelect++;
                    switch (wifiSettingGroupItemsSelect)
                    {
                    case 1:
                        wheatherSettingWifiName = true;/*如果选中设置wifi名称，就可以下拉*/
                        number = ui->lWIFIName_le->currentText();
                        settingStyle();
                        ui->lWIFIName_le->setStyleSheet("border:1px solid green;"
                                                        "font: 18pt \"宋体\";"

                                                        "border-color: rgb(255, 0, 0);");

                        break;
                    case 2:
                        wheatherSettingWifiName = false;
                        number = ui->WIFIPassword_le->text();
                        settingStyle();
                        ui->WIFIPassword_le->setStyleSheet("border:1px solid green;"
                                                           "font: 18pt \"宋体\";"

                                                           "border-color: rgb(255, 0, 0);");

                        break;
                    case 3:
                        wheatherSettingWifiName = false;
                        number = ui->IPAdress_le->text();
                        settingStyle();
                        ui->IPAdress_le->setStyleSheet("border:1px solid green;"
                                                       "font: 18pt \"宋体\";"

                                                       "border-color: rgb(255, 0, 0);");
                        break;
                    case 4:
                        wheatherSettingWifiName = false;
                        number = ui->port_le->text();
                        settingStyle();
                        ui->port_le->setStyleSheet("border:1px solid green;"
                                                   "font: 18pt \"宋体\";"

                                                   "border-color: rgb(255, 0, 0);");
                        break;
                    default:
                        break;
                    }
                }
            }
        }
        else if(temp[0]==(char)28)
        {
            temp[0]=255;
            on_back_pb_clicked();
        }
        else if(temp[0]==(char)34)
        {
            temp[0]=255;
            if(wheatherSettingWifiName) // shezhi wifi name
            {
                if(showOrHideWifiNameDrop)
                {
                    showOrHideWifiNameDrop = false;
                    ui->lWIFIName_le->showPopup();
                    wheatherOpenSettingWifiName = true; //dakai  wifi name shezhi
                }
                else
                {
                    showOrHideWifiNameDrop = true;
                    ui->lWIFIName_le->hidePopup();
                    ui->lWIFIName_le->setCurrentIndex(wifiNameIndex);
                    wheatherOpenSettingWifiName = false;
                }

            }
        }
        else if(temp[0]==(char)2)
        {
            temp[0]=255;
            ui->remoteControl_cb->click();
            quint8 checkState;
            if(ui->remoteControl_cb->isChecked())
            {
                checkState = 1;
            }
            else
            {
                checkState = 0;
            }
            QFile fileRemoteCheck("/xrfile/RemoteIni.conf");
            if(fileRemoteCheck.open(QIODevice::ReadWrite))
            {
                QDataStream out(&fileRemoteCheck);
                out<<checkState;
                fileRemoteCheck.flush();
                fileRemoteCheck.close();
                system("sync");
            }
        }
        else if(temp[0] == (char)1)
        {
            temp[0]=255;
            ui->checkBox->click();
        }
        else if(temp[0] == (char)3)//F3按钮
        {
            temp[0] = 255;
            on_autoConn_btn_clicked();
        }
        else if(temp[0] == (char)37)
        {
            temp[0] =255;
            if(wifiSettingGroupItemsSelect <3)
            {
                Softwear soft;
                connect(&soft,SIGNAL(setNumber(QString)),this,SLOT(getWifiPassord(QString)));
                soft.exec();
            }

        }
        else if(temp[0]==(char)27)
        {
            temp[0]=255;
            switch (wifiSettingGroupItemsSelect)
            {
            case 0:
                ui->MachID_lb->setText("");
                break;
            case 1:

                break;
            case 2:
                ui->WIFIPassword_le->setText("");
                break;
            case 3:
                ui->IPAdress_le->setText("");
                break;
            case 4:
                ui->port_le->setText("");
                break;
            default:
                break;
            }
        }
        else if(temp[0]==(char)12)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("1", number);
        }
        else if(temp[0]==(char)13)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("4", number);

        }
        else if(temp[0]==(char)14)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("7", number);
        }
        else if(temp[0]==(char)17)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("2", number);
        }
        else if(temp[0]==(char)18)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("5", number);
        }
        else if(temp[0]==(char)19)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("8", number);
        }
        else if(temp[0]==(char)20)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("0", number);
        }
        else if(temp[0]==(char)22)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("3", number);
        }
        else if(temp[0]==(char)23)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("6", number);
        }
        else if(temp[0]==(char)24)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("9", number);
        }
        else if(temp[0]==(char)25)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput(".", number);
        }
        else if(temp[0]==(char)72)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("a", number);
        }
        else if(temp[0]==(char)77)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("b", number);
        }
        else if(temp[0]==(char)82)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("c", number);
        }
        else if(temp[0]==(char)73)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("d", number);
        }
        else if(temp[0]==(char)78)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("e", number);
        }
        else if(temp[0]==(char)83)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("f", number);
        }
        else if(temp[0]==(char)74)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("g", number);
        }
        else if(temp[0]==(char)79)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("h", number);
        }
        else if(temp[0]==(char)84)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("i", number);
        }
        else if(temp[0]==(char) 80)
        {
            temp[0]=255;
            setNumber();
            dealWithNumberInput("j", number);
        }
    }
}

/**处理键盘输入的数据**/
void WIFIMode::dealWithNumberInput(QString num, QString N1)
{
    number_answer(N1);
    //wifiItemFlag_lb->setText(N1.append(num));
    switch (wifiSettingGroupItemsSelect)
    {
    case 0:
         ui->MachID_lb->setText(N1.append(num));
        break;
    case 1:

        break;
    case 2:
         ui->WIFIPassword_le->setText(N1.append(num));
        break;
    case 3:
         ui->IPAdress_le->setText(N1.append(num));
        break;
    case 4:
         ui->port_le->setText(N1.append(num));
        break;
    default:
        break;
    }
}

/**处理键盘输入的数据**/
void WIFIMode::number_answer(QString& n)
{
    if(digital_mark==0)
    {
        n="";
        digital_mark=1;
    }
}

/**是否选择自动连接**/
void WIFIMode::on_checkBox_clicked()
{
    myWiFi->my_WiFi.auto_Connect=ui->checkBox->isChecked();
    //连接时，需要将网络配置信息保存，
    QSettings *set=new QSettings(IOM_PATH,QSettings::IniFormat);
    set->setValue("WiFiParameter/AutoConnect",ui->checkBox->isChecked()==true?1:0);
    delete set;
}

/*
 * 连接按钮autoConn_btn对应的单击事件
 * 如果按钮文本为断开连接，则首先需要保存网络配置信息，然后退出透传模式，并重启WIFI模块
 * 如果按钮文本为连接，则开启定时器3，并退出透传模式，发AT测试指令
 * loginStation置位LOGIN_BEFORE;
*/
void WIFIMode::on_autoConn_btn_clicked()
{
    if(ui->autoConn_btn->text()==tr("连接WiFi(F2)"))
    {
        if(ui->lWIFIName_le->currentText()=="")
            return;
        WiFiParameter par;
        par.machineID=ui->MachID_lb->text();
        par.WiFi_Name=ui->lWIFIName_le->currentText();
        par.WiFi_Password=ui->WIFIPassword_le->text();
        par.WiFi_IP=ui->IPAdress_le->text();
        par.WiFi_Port=ui->port_le->text();
        par.auto_Connect=ui->checkBox->isChecked();
        par.remote_Operation=ui->remoteControl_cb->isChecked();
        //连接时，需要将网络配置信息保存，
        QSettings *set=new QSettings(IOM_PATH,QSettings::IniFormat);
        set->setValue("WiFiParameter/MachineID",par.machineID);
        set->setValue("WiFiParameter/WiFiName",par.WiFi_Name);
        set->setValue("WiFiParameter/WiFiPassword",par.WiFi_Password);
        set->setValue("WiFiParameter/SeverIP",par.WiFi_IP);
        set->setValue("WiFiParameter/SeverPort",par.WiFi_Port);
        set->setValue("WiFiParameter/AutoConnect",par.auto_Connect==true?1:0);
        set->setValue("WiFiParameter/Remote",par.remote_Operation==true?1:0);
        delete set;
        system("sync");
        myWiFi->connentWiFi_clicked(par);

    }
    else if(ui->autoConn_btn->text()==tr("断开连接"))
    {
        ui->autoConn_btn->setText("正在断开...");
        myWiFi->closeWiFi();
    }
}

void WIFIMode::autoScroll()
{
    QTextCursor cursor =  ui->textBrowser->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->textBrowser->setTextCursor(cursor);
}

void WIFIMode::settingStyle()
{
    ui->MachID_lb->setStyleSheet("border:1px solid green;"
                                 "font: 18pt \"宋体\";"

                                 "border-color: rgb(255, 255, 255);");


    ui->lWIFIName_le->setStyleSheet("border:1px solid green;"
                                 "font: 18pt \"宋体\";"

                                 "border-color: rgb(255, 255, 255);");


    ui->WIFIPassword_le->setStyleSheet("border:1px solid green;"
                                 "font: 18pt \"宋体\";"

                                 "border-color: rgb(255, 255, 255);");


    ui->IPAdress_le->setStyleSheet("border:1px solid green;"
                                 "font: 18pt \"宋体\";"

                                 "border-color: rgb(255, 255, 255);");


    ui->port_le->setStyleSheet("border:1px solid green;"
                                 "font: 18pt \"宋体\";"

                                 "border-color: rgb(255, 255, 255);");

}


void WIFIMode::setNumber()
{
     switch (wifiSettingGroupItemsSelect)
     {
     case 0:
         number = ui->MachID_lb->text();
         break;
     case 1:

         break;
     case 2:
        number = ui->WIFIPassword_le->text();
         break;
     case 3:
        number = ui->IPAdress_le->text();
         break;
     case 4:
         number= ui->port_le->text();
         break;
     default:
         break;
     }
}

void WIFIMode::getWifiPassord(QString pwd)
{
    if(wifiSettingGroupItemsSelect==1)
    {
        ui->lWIFIName_le->addItem(pwd);
    }
    else if(wifiSettingGroupItemsSelect==0)
    {
       ui->MachID_lb->setText(pwd.left(4));
    }
    else
    {
         ui->WIFIPassword_le->setText(pwd);
    }
}

void WIFIMode::machIdClickSlot()
{
    wifiSettingGroupItemsSelect = 0;
    settingStyle();
    ui->MachID_lb->setStyleSheet("border:1px solid green;"
                                    "font: 18pt \"宋体\";"

                                    "border-color: rgb(255, 0, 0);");

}

void WIFIMode::wifiPwdClickSlot()
{
    wifiSettingGroupItemsSelect = 2;
    settingStyle();
    ui->WIFIPassword_le->setStyleSheet("border:1px solid green;"
                                    "font: 18pt \"宋体\";"

                                    "border-color: rgb(255, 0, 0);");
}

void WIFIMode::tcpAdressClickSlot()
{
    wifiSettingGroupItemsSelect = 3;
    settingStyle();
    ui->IPAdress_le->setStyleSheet("border:1px solid green;"
                                    "font: 18pt \"宋体\";"

                                    "border-color: rgb(255, 0, 0);");
}

void WIFIMode::tcpPortClickSlot()
{
    wifiSettingGroupItemsSelect = 4;
    settingStyle();
    ui->port_le->setStyleSheet("border:1px solid green;"
                                    "font: 18pt \"宋体\";"

                                    "border-color: rgb(255, 0, 0);");
}

void WIFIMode::on_lWIFIName_le_highlighted(const QString &arg1)
{
    wifiSettingGroupItemsSelect = 1;
    settingStyle();
    ui->lWIFIName_le->setStyleSheet("border:1px solid green;"
                                    "font: 18pt \"宋体\";"

                                    "border-color: rgb(255, 0, 0);");
}

void WIFIMode::on_remoteControl_cb_clicked()
{
    myWiFi->my_WiFi.remote_Operation=ui->remoteControl_cb->isChecked();
    //连接时，需要将网络配置信息保存，
    QSettings *set=new QSettings(IOM_PATH,QSettings::IniFormat);
    set->setValue("WiFiParameter/Remote",ui->remoteControl_cb->isChecked()==true?1:0);
    delete set;
}

quint32 WIFIMode::GetCRC32(unsigned char* buf, int len)
{
    quint32 crc;
    crc = 0xFFFFFFFF;
    for (int i = 0; i <len; i++)
        crc = Crc32Table[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
    return crc^0xFFFFFFFF;
}

bool WIFIMode::CheckCRC32(unsigned char*buf, const int &length)
{
    if(length>0)
    {
        quint32 Calculate_CRC32 = GetCRC32(buf,length-5);
        unsigned char crc_re[4];
        for(int i = 0; i<4; i++)
        {
            crc_re[i]=buf[length-5+i];
        }
        quint32 received_CRC32 = ((crc_re[0]<<8|crc_re[1])<<16)|(crc_re[2]<<8|crc_re[3]);
        if(Calculate_CRC32==received_CRC32)
            return true;
    }
    return false;
}

void WIFIMode::getWiFiParameter(WiFiParameter par, int index)
{
    ui->lWIFIName_le->setCurrentIndex(index);
    ui->MachID_lb->setText(par.machineID);
    ui->WIFIPassword_le->setText(par.WiFi_Password);
    ui->IPAdress_le->setText(par.WiFi_IP);
    ui->port_le->setText(par.WiFi_Port);
    ui->checkBox->setChecked(par.auto_Connect);
    ui->remoteControl_cb->setChecked(par.remote_Operation);
}

void WIFIMode::getWiFiName(QStringList sl)
{
    ui->lWIFIName_le->clear();
    ui->lWIFIName_le->addItems(sl);
}

void WIFIMode::getWiFiString(QString str)
{
    if(str=="CONNECTING")
    {
        ui->autoConn_btn->setText("正在连接...");
    }
    else if (str=="CONNECTED") {
        ui->autoConn_btn->setText("断开连接");
    }
    else if (str=="DISCONNECTED") {
        ui->autoConn_btn->setText("连接WiFi(F2)");
    }
    else if(str.contains("dbm"))
    {
        QString dbm= str.remove(0,4);
        //connectProgress=dbm.toInt();
    }
    else if (str.contains("Status")) {

    }
    else if (str.contains("Msg:")) {
        m_Info->DisErrorMsg(QMessageBox::Warning,str.remove(0,4));
    }
    else if (str.contains("Total_yield:")) {
        realtime_parameter.curyield=str.remove(0,12).toInt();
    }
}

void WIFIMode::getData(QByteArray data, int i)
{
//    if(i==0)
//    {
//        QString strDisplay = "【"+QDateTime::currentDateTime().toString("HH:mm:ss")+"】"+" ← 发 "+ data;
//        ui->textBrowser->insertPlainText(strDisplay);
//    }
//    else {
//        QString strDisplay ="【"+ QDateTime::currentDateTime().toString("HH:mm:ss")+"】"+" → 收 "+ data;
//        ui->textBrowser->insertPlainText(strDisplay);
//    }
}

QString WIFIMode::fourBytesDatas(QString str)
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

QString WIFIMode::twentyBytesData(QString ch)
{
    QString twentyBytes("0000000000000000000000000000000000000000");
    twentyBytes.chop(ch.length());
    return twentyBytes.append(ch);
}

void WIFIMode::sendData(QByteArray qba)
{
    if(myWiFi->mycom_WiFi.isOpen()&&myWiFi->WiFiConnected)
    {
        myWiFi->sendData(qba);
    }
}
