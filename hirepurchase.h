#ifndef HIREPURCHASE_H
#define HIREPURCHASE_H
#include "hirethread.h"
#include <QWidget>

class Hirepurchase : public QWidget
{
    Q_OBJECT

public:
    explicit Hirepurchase(QWidget *parent = 0);
    ~Hirepurchase();
    bool startHirpurchase(QString deadline);
    bool sysetemSetting(QString filename);//配置系统
    int unlockedAuto(QString sysId,QString cor,QString keyOraddr);//
    QString createRandomCode();//手动解锁时，生产随机码
    bool produceKeyCode(QString KeyCode);//输入密钥后的比对结果
    void setData(QString length, QString code);
    QString decryption(QString str);
    QString sysID;//系统id
    QString sysDeadlineTime;//系统到期时间
    QString sysStartTime;//系统起始时间
    QString companyCode;
    QString mac;
    int companyCodeLength;
    QString companyCodeAscii;
    QString companyCodeAsciiLength;
    QString companyCodeEnding;
    QString sysDeadlineTimeEnding;
    QString sysIDEnding;
    QString macEnding;

    int macLength;
    int idLength;
    QString id;
    int startTLength;
    int endTLength;

    QString handCode;
   // Mythread *mythread;
    hireThread *mythread;
signals:
    void setExpireReminder(bool);//发送是否到期的信号到调用该方法的外部类   到期（true）、未到期（false）

public slots:
    void getInfoWhetherExpire(bool);//获取线程中的到期信号
private:
    bool compareDate(QString date);
    qint32 HigherLowerConvert(qint32 original);//高低位互换
    void analyzeFile(QString filepath);
    QString decryptKey(QByteArray,QByteArray,QByteArray);
    QByteArray hexToString(QString str);
    QString fileNameStart;
    int analyseFlag;
    QString result;
    QString encryption();
};

#endif // HIREPURCHASE_H
