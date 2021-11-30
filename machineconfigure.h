#ifndef MACHINECONFIGURE_H
#define MACHINECONFIGURE_H

#include <QDialog>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include "myheader.h"

enum configureType{
    typeui,
    typertc,
};

namespace Ui {
class MachineConfigure;
}

class MachineConfigure : public QDialog
{
    Q_OBJECT

public:
    explicit MachineConfigure(QWidget *parent = 0);
    ~MachineConfigure();
    void setconfigureType(configureType type);
    void setID(QString id);

private slots:
    void on_pushButton_read_clicked();

    void on_pushButton_configure_clicked();

    void on_pushButton_save_clicked();

    void timeout();

    void on_pushButton_back_clicked();

private:
    Ui::MachineConfigure *ui;
    int keymarkTemp;
    quint8 id[10];
    QString machineID;
    QString companyCode;//公司名称
    QString companyCodeAscii;//公司名称ASCII码值
    QString companyCodeAsciiLength;//公司名称ASCII码值长度
    QString startTime;
    QString endTime;
    QTimer *timer1;
    configureType confType;
    bool accepted;
    bool readed;
    bool issetting;
    int timecount;
};

#endif // MACHINECONFIGURE_H
