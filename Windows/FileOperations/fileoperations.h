#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <QDialog>
#include <QDir>
#include <QDebug>
#include <QFileSystemWatcher>
#include <QFileSystemModel>
#include <QFileInfo>
#include <QFileInfoList>
#include <QString>
#include <QDateTime>
#include <QTimer>
#include <QTableWidgetItem>
#include <QFile>
#include "Windows/FileOperations/sumneedleerror.h"
#include "Windows/FileOperations/resetornot.h"
#include <QFileSystemWatcher>
#define NeedleLocation "/xrfile/auxiliaryNeedle.er"
#include "tucos.h"
#include "error/errorcode.h"
#include <QSettings>
#include "myheader.h"
#ifdef Q_OS_LINUX
#include <unistd.h>
#endif

struct action{
    quint16 quan;
    quint16 row;
    quint16 needle;
    quint16 value;
    QList<quint16> values;
};

struct dismotor{
    quint32 startStep;
    quint32 endStep;
    quint32 startAddr;
    quint32 endAddr;
    quint16 height;
    quint16 width;
    QList<action*> actions;
    QList<action*> lisact;
};

//以前的色纱格式
struct yarnAction{
    quint16 quan;
    quint16 row;
    quint16 needle;
    quint8 value[3];
};

//色纱新格式
struct yarnNewAct
{
    quint16 quan;
    quint8 count;
    QList<quint8> values;
};

struct disYarn
{
    quint32 startStep;
    quint32 endStep;
    quint32 startAddr;
    quint32 endAddr;
    QList<yarnAction*> actions;
    QList<yarnNewAct*> newActs;
};

struct mot{
    quint16 startstep;
    quint16 endstep;
    quint16 data[16];
};

struct speedInfo
{
    quint16 step[70];
    quint16 speed[70];
    quint16 ramp[70];
};

struct ecoInfo
{
    quint16 step_s[50];
    quint16 step_e[50];
    quint16 value[50];
};

enum TableType{
    root,
    myusb
};

namespace Ui {
class FileOperations;
}

class FileOperations : public QDialog
{
    Q_OBJECT

public:
    explicit FileOperations(QWidget *parent = 0);
    ~FileOperations();
    void showinfo_DISK();
    bool fileSpeed_extract(QString filename);//提取花型运行速度值
    bool fileNeedle_extract(QString filename);//提取花型压针值
    bool fileCirculation_extract(QString filename);//提取花型循环信息
    int LH_deal(int);
    int deal_Bit32(int valve);

private slots:
    void showinfo_USB();
    void on_back_pb_clicked();
    void on_filedelete_File_pb_clicked();
    void on_copytoUSB_File__pb_clicked();
    void on_copytodisk_File_pb_clicked();
    void on_flush_file_pb_clicked();
    void on_patterndownload_pb_clicked();
    void update_slot();
    void deleteFileSlot(bool flag);
    void on_pushButton_2_clicked();
    void usbCellClick(int row,int col);
    void diskCellClick(int row,int col);

signals:

private:
    Ui::FileOperations *ui;
    QTableWidget *table;
    QTimer *timer_fileoperations;
    QString filename_transmit;
    QString filename_transmit_uco;
    TableType tabletype;
    QString filepath_DISK_transmit_uco;
    QString filepath_DISK_transmit;
    int totalPacks;//花型总包数
    int totalBytes;//花型总字节数
    int timecount;//下载超时计时
    int totalStep;//CO总步骤数
    int guidPosition[8];//8路色纱机型位置
    int yarnFingerAngle[8];
    QList<int> listneedle;
    QStringList filters;
    QFileSystemWatcher *watcher;
    void transPat();
    bool downLoadFlag;
    bool patterndowning;//花型正在下载
    bool patterndownloaded;//花型下载完成
    bool firstEnter;
    QList<mot> listmot;
    ErrorCode *m_pMsg;
    speedInfo speedinfo;
    ecoInfo ecoinfo;
    int dealCO(QString pathco);
    int dealUCO(QString pathuco);
    int readluoan(QString name);
    quint32 deal_4bytes(quint32 value);
    quint16 deal_2bytes(quint16 value);
    void initialForm();
    void tableChange(TableType type);
    static bool compareBarData(const yarnAction *act1,const yarnAction *act2);
};

#endif // FILEOPERATIONS_H
