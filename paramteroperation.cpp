#include "paramteroperation.h"
#include "myheader.h"
#include <QFile>

ParamterOperation::ParamterOperation()
{

}

//保存参数文件
void ParamterOperation::machine_par_save()
{
    QFile file(Path_parameterfile);
    if(file.open(QIODevice::ReadWrite))
    {
        QDataStream in(&file);
        quint16 *data;
        data=&machine_Pars.needletype;
        for(int i=0;i<parameter_Count;i++)
            in<<data[i];
        file.close();
        system("sync");
    }
}

//打开参数文件
void ParamterOperation::machine_par_open()
{
    QFile file(Path_parameterfile);
    if(file.open(QIODevice::ReadWrite))
    {
        QDataStream out(&file);
        for(int i=0;i<parameter_Count;i++)
            out>>*(&machine_Pars.needletype+i);
        file.close();
    }
}

//保存系统日志 最多200条
void ParamterOperation::saveSystemLog(QString msg, QDateTime t)
{
    QSettings *set=new QSettings("/xrfile/system.log",QSettings::IniFormat);
    int curIndex=set->value("Log/Index","0").toInt();
    curIndex++;
    if(curIndex>500)
        curIndex=1;
    QString key=QString("Log/Msg_%1").arg(curIndex);
    set->setValue(key,QString("[%1] - %2").arg(t.toString("yyyy-MM-dd HH:mm:ss")).arg(msg));
    set->setValue("Log/Index",curIndex);
    delete set;
#ifdef Q_OS_LINUX
    system("sync");
#endif
}
