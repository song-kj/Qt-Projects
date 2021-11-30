#ifndef CONTROLUI_H
#define CONTROLUI_H

#include <QDialog>
#include <QDir>
#include <QFile>
#include <QLabel>
#include <QTimer>
#include <QTableWidgetItem>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QScrollBar>
#include <QFileSystemWatcher>
#include "mymessagebox.h"
#include "myheader.h"
#define Vision_ID "/xrfile/Vision_Control"
namespace Ui {
class controlUI;
}

enum ProgramType{
    MainCtl,
    StepMotor,
    AirValve,
    Actuator,
    PYF,
};

class controlUI : public QDialog
{
    Q_OBJECT
    
public:
    explicit controlUI(QWidget *parent = 0);
    ~controlUI();
    void setProgramType(ProgramType type);

private slots:
    void on_copyToUSB_pb_clicked();//考入usb槽函数
    void on_copyToDISK_pb_clicked();//考入本地槽函数
    void on_programUpdate_pb_clicked();//升级槽函数
    void on_pushButton_4_clicked();//返回槽函数
    void GetQTableWidgetObject(QTableWidgetItem*);//响应某一item被选中
    void on_delete_pb_clicked();//删除槽
    void update_slot();//定时器槽
     void getUSB(QString);
     void on_pushbutton_sub_clicked();

     void on_pushbutton_add_clicked();

signals:
    void sendCoverFileMark(int);
    void sendfeedback(int index,int b);

private:
    Ui::controlUI *ui;
    QTableWidget *table;//用来标识被选择的是本地QTableWidget还是USBQTableWidget
    ProgramType proType;
    quint8 proTypeNum;
    int totalPacks;//花型总包数
    int totalBytes;//花型总字节数
    bool firstEnter;
    bool isdowning;
    bool filetransfercomplete;
    int timecount;
    quint8 feeder;
    quint8 maxfeeder;
    QStringList filters;//文件过滤器
    QTimer *timer_control;
    QString filename;
    QString transname;
    int exchangeMark;
    QFileSystemWatcher *watcher;
    void initialForm();
    void  showinfo_DISK();//显示本地升级文件
    void  showinfo_USB();//显示USB升级文件
};

#endif // CONTROLUI_H
