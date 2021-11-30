#ifndef XRWIFI_H
#define XRWIFI_H
#include <QObject>
#include <QIODevice>
#include <QCom/qextserialport.h>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QStringList>
#include <QTimer>
#include <QThread>
#include <QMessageBox>

//WiFi参数
struct WiFiParameter
{
    QString machineID;//设置编号
    QString WiFi_Name;//WiFi名称
    QString WiFi_Password;//WiFi密码
    QString WiFi_IP;//IP地址
    QString WiFi_Port;//端口
    bool auto_Connect;//是否自动连接
    bool remote_Operation;//是否支持远程操作
    bool global_locked;//是否锁机
};

//WiFi串口参数
struct MyCom
{
    QString name;//串口名
    BaudRateType baudratetype;//波特率

};

//动态数据
struct DynamicData
{
    qint16 needles;//当前针位
    qint16 angle;//当前角度
    qint16 speed;//当前速度
    qint16 selAdjust;//选针调节
    qint16 fixNeedle;//固定出针
    qint16 dynSpeed;//点动速度
    qint16 limSpeed;//限速度值
    qint16 maxSpeed;//最大速度
    QString run_speed;//运行速度
    qint16 injInterval;//喷油间隔
    qint16 injNumbers;//喷油次数
    QString op_state;//油泵状态
    QString ele_switch;//电磁开关
    QString up_lamp;//上照布灯
    QString down_lamp;//下照布灯
    QString flu_lamp;//日光灯
    QString fan;//风扇
    qint16 fineTuning[8];//压针微调
    quint16 cur_yield;//当前产量
    quint16 set_yield;//设定产量
    quint16 total_yield;//总产量
    qint16 single_time;//单件时间
    QString duty_id;//值班人员ID
    QString pro_name;//工程名称
    qint16 total_step;//总步骤数
    qint16 cur_step;//当前步骤
    qint16 total_circle;//总圈数
    qint16 cur_circle;//当前圈数
};

enum WIFI_typedef{
    WIFI_NONE,
    WIFI_MAC,
    WIFI_MODE,
    WIFI_SCAN,
    WIFI_RESET,
    WIFI_TEST,
    WIFI_CONNECT,
    WIFI_CONNECTED,
    TOUCHUAN_SETTING,
    TCP_CONNECTED,
    TCP_SEND,
    TOUCHUAN_STARTING,
    TCP_DISCONNECT,
    TCP_CLOSE,
    WIFI_DISCONNECT,
    WIFI_CLOSE,
};

//机器状态
enum Machine_Status{
    Run,//运行
    Stop,//停车
    On,//开机
    Off,//关机
    Alarm,//报警
    AlarmClear,//报警消除
};

class XRWiFi:public QObject
{
    Q_OBJECT
public:
    explicit XRWiFi(QObject *parent = 0);
    QextSerialPort mycom_WiFi;//定义WiFi串口
    WiFiParameter my_WiFi;//定义WiFi参数
    bool setWiFiComPort(MyCom com);//设置WiFi串口并打开串口,返回成功或失败
    bool WiFiConnected;//WiFi是否是否连接标志位
    int connectProgress;//WiFi连接状态
    DynamicData dynamicData;//动态数据
    Machine_Status machineState;
    QString curTime();
    void closeWiFiComPort();//关闭WiFi串口
    void setWiFiParameter(WiFiParameter par);//设置WiFi连接参数
    void sendData(QByteArray data);//向WiFi串口发送数据
    void readFileOffWIFIPAR();//读取WiFi参数文件
    void connentWiFi_clicked(WiFiParameter par);//连接WiFi
    void machineStatusSwitch(Machine_Status state);//机运行状态切换
    void closeWiFi();//断开WiFi连接
    void writeAlarm(int num1,int num2);//向服务器写报警

    int restsum;

private:
    WIFI_typedef wifiType;

    QByteArray receiveData;//接收缓存
    QByteArray sendBuffer;//发送缓存
    QString macAdress;//Mac地址
    QString fourBytesDatas(QString str);
    QString twentyBytesData(QString ch);
    int wifiConnectCount;//WiFi连接次数，5次后重启
    int wifiRebootCount;//重启次数
    int baojingceshi;//发报警测试
    int wifiStatusCount;
    int restartCount;
    int lianjiCount;
    int atreceiveCount;//AT指令回复计数
    int timerInterval;
    bool receive_at;
    bool sendStaticPar;
    bool restart;
    bool wifiScaned;//WiFi热点扫描完成
    bool exitTouChuanrequest;//退出透传请求
    int exitTouChuanCount;
    QTimer *timer1;//定时器1，用于发送动态数据
    QTimer *timer3;//定时器3，用于检测网络中断
    void MySleep(unsigned  int msec);
    void WiFiConnect(bool request);//获得Mac地址，并扫描当前可用热点
    void WiFiScan();
    void sendStaticData();//发送静态数据
    void update_time(const QString &s_time);//根据服务器的时间校准系统时间
    void LockDevice(QString passwd);//远程锁机
    void UnlockDevice(QString passwd);//远程解锁
    void setYield();//产量设定
    void restartWiFi();//重启WiFi模块
    void sendATinstruction(QString str);//发送AT指令


private slots:
    void getData();//读取串口数据
    void sendDynamicData();//发送动态数据
    void WiFiStatus();//检测网络状态

signals:
    void sendWiFiData(QByteArray data,int i);//发送串口接收到的数据
    void sendWiFiParameter(WiFiParameter par,int i);
    void sendQString(QString str);
    void sendQStringList(QStringList sl);
};

#endif // XRWIFI_H
