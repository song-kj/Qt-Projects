#ifndef HIRETHREAD_H
#define HIRETHREAD_H
#include <QThread>

class hireThread:public QThread
{
    Q_OBJECT
public:
    explicit hireThread(QObject *parent = 0);


protected:
   void run();
private:
   bool compareDate(QString date);

signals:
   void setInfoWhetherExpire(bool);//发送是否到期的信号到调用该线程的类

};

#endif // HIRETHREAD_H
