#ifndef QLUOANANGLE_H
#define QLUOANANGLE_H

#include <QWidget>
#include <QDialog>
#include <QFile>
#include <QDataStream>
#include <QStandardItemModel>
#include <QTableView>
#include <QDebug>
#include "math.h"
#include <QTimer>
#include "myheader.h"

namespace Ui {
class QLuoAnAngle;
}

class QLuoAnAngle : public QDialog
{
    Q_OBJECT

public:
    explicit QLuoAnAngle(QWidget *parent = 0);
    ~QLuoAnAngle();

private:
    Ui::QLuoAnAngle *ui;
    int max_row;
    int count;
    int focused_row;
    int focused_col;
    bool downReceive;
    bool startDownload;
    quint16 data[100][17];
    QTimer *keyboard_timer;
    void InitTable();
    void display_table();
    void ReadPara();//进入裸氨设置界面时读取裸氨参数
    void read_luoan_file(const QString &name);
    void SavePara();
    void SetPara(const QString &str_para);   //设置表格参数：裸氨调节值
    quint16 deal_Bit16(quint16 value);

private slots:
    void cellclick(int i,int j);
    void InputEvent();//处理键盘的输入
    void on_download_btn_clicked();
    void on_save_btn_clicked();
    void on_return_btn_clicked();
    void transParameter();
};

#endif // QLUOANANGLE_H
