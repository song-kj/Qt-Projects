#include "patternwoven.h"
#include <QApplication>
#include "Communication/doubleportram.h"
#include <QFont>
#include <QTextCodec>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QSettings>
#include "tucos.h"
QString ui_name;
QString global_MainControl_Version;//主控程序版本号

void create_dir()
{
    QDir dir;
    if(!dir.exists("/home/root"))
    {
        dir.mkdir("/home/root");
    }
    if(!dir.mkdir("/home/update_file"))
    {
        dir.mkdir("/home/update_file");
    }
    if(!dir.exists("/xrfile"))
    {
        dir.mkdir("/xrfile");
    }
    if(!dir.exists("/xrfile/alarm"))
    {
        dir.mkdir("/xrfile/alarm");
    }
    //判断系统文件是否丢失 丢失拷贝备份文件
    if(QFile::exists(Path_parameterfile)==false)
    {
        if(QFile::exists(Path_parameterfile_backup))
        {
            if(QFile::copy(Path_parameterfile_backup,Path_parameterfile))
            {
                system("sync");
            }
        }
    }
}

void program_version()
{
    //读取配置文件，获得UI程序名称（默认为“XRUI”）
    QSettings *set=new QSettings("/xrfile/Machine.ini",QSettings::IniFormat);
    ui_name = set->value("UI/AppName","XRUI").toString();
    delete set;

    QFile file_MainCL("/xrfile/Vision_Control");
    if(file_MainCL.open(QIODevice::ReadWrite))
    {
        QTextStream in(&file_MainCL);
        in>>global_MainControl_Version;
        file_MainCL.close();
    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("cleanlooks"));
    QTextCodec *codec = QTextCodec::codecForName("Utf8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
    create_dir();
    program_version();
    PatternWoven w;
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.show();
    return a.exec();
}
