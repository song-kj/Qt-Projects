#ifndef WIFIMODE_H
#define WIFIMODE_H

#include <QDialog>
#include "QCom/qextserialport.h"
#include <QTimer>
#include "Windows/WIFI/patternoperation.h"
#include "Windows/Settings/parametersaveinfo.h"
#include <QLabel>
#include "error/errorcode.h"
#include "error/neworder.h"
#include <QSettings>
#include "xrwifi.h"

#define PASSWRD_FROMSERVER "/xrfile/passwrd.txt"
#define DEVICE_LOCK_STA "/xrfile/lock.txt"
#define MAC_ADDR_PATH "/xrfile/mac_addr.txt" //MAC地址文件路径
namespace Ui {
class WIFIMode;
}

class WIFIMode : public QDialog
{
    Q_OBJECT

public:
    explicit WIFIMode(QWidget *parent = 0);
    ~WIFIMode();

    QString fourBytesDatas(QString str);

    QString twentyBytesData(QString ch);

    void sendData(QByteArray qba);
    XRWiFi *myWiFi;
signals:
    void sendProgressData(int,int,QString);

    void update_MachineID();

    void single_stop_signal();//单停指令

private slots:

    void on_back_pb_clicked();

    void update_slot();//接收键盘数据的事件

    void dealWithNumberInput(QString, QString);

    void number_answer(QString&);

    void on_checkBox_clicked();

    void on_autoConn_btn_clicked();

    void autoScroll();
    void getWifiPassord(QString);

    void machIdClickSlot();

    void wifiPwdClickSlot();

    void tcpAdressClickSlot();

    void tcpPortClickSlot();

    void on_lWIFIName_le_highlighted(const QString &arg1);

    void on_remoteControl_cb_clicked();

    //init_CRC32_table只需要调用一次，用于生成CRC32_table，
    //也可以事先计算好CRC32_table中的每一项，然后直接初始化数组：
    quint32 GetCRC32(unsigned char *buf, int len);//计算CRC校验码

    bool CheckCRC32(unsigned char *buf, const int &length);//校验CRC

    void getWiFiParameter(WiFiParameter par,int index);
    void getWiFiName(QStringList sl);//获取当前可用热点名称
    void getWiFiString(QString str);//获得WiFi传递的字符串数据
    void getData(QByteArray data,int i);
private:
    Ui::WIFIMode *ui;

    MyCom myCom;
    int connectSetting;
    QTimer *pQTimer;//扫描键盘输入的定时器
    ErrorCode *m_Info;
    int digital_mark;
    int wifiSettingGroupItemsSelect;
    PatternOperation *pPatternOperation;
    bool wheatherSettingWifiName;
    bool wheatherOpenSettingWifiName;
    int wifiNameIndex;
    QString  number;
    bool showOrHideWifiNameDrop;

    /**************************************/
    bool comIsOpend;
    void settingStyle();
    void setNumber();

};

#endif // WIFIMODE_H
