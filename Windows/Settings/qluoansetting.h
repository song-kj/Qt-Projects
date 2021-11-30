#ifndef QLUOANSETTING_H
#define QLUOANSETTING_H

/* 文件名：QLuoAnSetting
 * 作者：梅再欢
 * 日期：2018-01-18
 * 描述：裸氨设置，步数行最大50（不是步数数值），Elan 8路（Elan1-Elan8）,均为16位；
 *      路数分页显示，每一页10路，总路数根据配置文件确定；
 *      参数可以修改，最大值4096，也可以从主控读取；参数保存地址805H-1754H；
 *      开机时所有裸氨参数需要下传给主控，裸氨参数文件为/xrfile/Parameter.er，从1608地址处开始存数据
 * ***********************************************************************/

#include <QDialog>
#include <QStandardItemModel>
#include <QTableView>
#include <QSettings>
#include <QTimer>
#include <QFile>
#include <QScrollBar>
#include <map>
#include <iostream>
#include "myheader.h"

#define LUO_AN_LOG "/xrfile/luo_an.log"         //裸氨table行数配置文件
#define LUO_AN_PARA "/xrfile/Parameter_Luoan.er"        //裸氨参数保存文件路径
#define MAX_STEP 100                     //最大步骤数也即表格最大行数100
#define MAX_COL 18                     //列数18：起始步+结束步+8*（ELAN起始+ELAN结束）

namespace Ui {
class QLuoAnSetting;
}

class QLuoAnSetting : public QDialog
{
    Q_OBJECT

public:
    explicit QLuoAnSetting(QWidget *parent = 0);
    ~QLuoAnSetting();
    void InitTable(); //初始化表格
    int ReadIni();   //读取配置文件，确定总路数
    void WriteIni(const int &row);  //修改总路数，写配置文件
    void SetPara(const QString &str_para);   //设置表格参数：裸氨调节值
    void SavePara();  //保存裸氨参数
    void ReadPara();//进入裸氨设置界面时读取裸氨参数
    void ShowDialog();//显示本界面
    void read_luoan_file(const QString &name);
    void display_table();

private slots:
    void on_return_btn_clicked();//返回上级菜单

    void on_save_btn_clicked();//保存按钮槽函数

    void InputEvent();//处理键盘的输入

    void on_setall_btn_clicked();

    void on_jia1_btn_clicked();

    void on_jian1_btn_clicked();

    void on_jia10_btn_clicked();

    void on_jian10_btn_clicked();

    void on_download_btn_clicked();
    void cellclick(int i,int j);

    void on_setrow_btn_clicked();

    void on_setcol_btn_clicked();

private:
    Ui::QLuoAnSetting *ui;
    QStandardItemModel *table_luoan;

    QTimer *keyboard_timer;
    bool setall;
    bool setrow;
    bool setcol;
    int current_pos;
    int count;
    int max_row;
    int max_index;
    int focused_row;
    int focused_col;
    bool downReceive;
    bool startDownload;
    struct Mot
    {
        quint16 startStep;
        quint16 endStep;
        quint16 motStart;
        quint16 motEnd;
    };
    std::map<int,Mot>MotorInfo[8];
    quint16 motorData[100][18];
};

#endif // QLUOANSETTING_H
