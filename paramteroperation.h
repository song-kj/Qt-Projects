#ifndef PARAMTEROPERATION_H
#define PARAMTEROPERATION_H

#include <QObject>
#include <QDateTime>
#include <QSettings>

class ParamterOperation
{
public:
    ParamterOperation();
    void machine_par_save();//保存机器参数
    void machine_par_open();//读取机器参数
    void saveSystemLog(QString msg,QDateTime t);//保存系统日志
};

#endif // PARAMTEROPERATION_H
