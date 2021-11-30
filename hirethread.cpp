#include "hirethread.h"
#include <QTime>
#include <QDebug>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
extern QString Deadline;
extern int Flag;
bool StartFlag;
hireThread::hireThread(QObject *parent) :
    QThread(parent)
{
    StartFlag=true;

}
void hireThread:: run()//线程中执行的内容
{
    while(StartFlag)
    {
        if(Flag==1)
        {
            if(compareDate(Deadline))
            {
                emit setInfoWhetherExpire(false);//未到期发送
            }
            else
            {
                Flag=0;
                emit setInfoWhetherExpire(true);//到期时发送
            }
        }
        sleep(600);
    }
}

bool hireThread::compareDate(QString date)
{
    QDateTime time = QDateTime::currentDateTime();
    QString currentTime = time.toString("yyyyMMdd");
    qDebug()<<"date"<<date<<"currentTime"<<currentTime;
    int year = date.left(4).toInt()-currentTime.left(4).toInt();
    if(year>0)
    {
        return true;
    }
    else if(year<0)
    {
        return false;
    }
    else
    {
        QString monthPreDate = date.remove(0,4);
        QString monthPreCurrentTime = currentTime.remove(0,4);
        int month = monthPreDate.left(2).toInt()-monthPreCurrentTime.left(2).toInt();
        if(month>0)
        {
            return true;
        }
        else if(month<0)
        {
            return false;
        }
        else
        {
            int day = date.right(2).toInt()-currentTime.right(2).toInt();
            if(day>0)
            {
               // qDebug()<<"day=="<<day;
                return true;

            }
            else
            {
                // qDebug()<<"day=="<<day;
                return false;
            }
        }
    }
}


