#ifndef MODIFYCO_H
#define MODIFYCO_H

#include <QDialog>
#include <QFile>
#include <QTimer>

struct SpeedANDEco{
    int step;
    int value;
    int ramp;
};

struct MotorInfo{
    quint16 degree;
    quint16 number;
    quint16 type;
    quint16 position;
};

struct chainmotor{
    quint16 step;
    quint16 degree;
    quint16 count;
    QList<quint16> value;
};

struct StepperMotor
{
    quint16 step;
    QList<MotorInfo> listMotor;
};


namespace Ui {
class ModifyCO;
}

class ModifyCO : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyCO(QWidget *parent = 0);
    ~ModifyCO();
    void settype(int t);

private slots:
    void on_btn_copy2usb_clicked();

    void on_toolButton_5_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_btn_copy2usb_2_clicked();

    void on_toolButton_6_clicked();

    void timeout();
    void singleslot();
private:
    Ui::ModifyCO *ui;
    QTimer *timer1;
    QString filename;
    int offsetAddr[10];
    QList<SpeedANDEco> speedinfo;
    QList<SpeedANDEco> ecoinfo;
    QList<StepperMotor> motorInfo;
    int type;
    quint16 totalStep;
    void initialForm();
    void refreshCOdata();
    bool readcodata(QString filename);
    void savecodata(QString filename);
    quint16 change2bytesdata(quint16 value);
    quint32 change4bytesdata(quint32 value);
    QString getTypeString(int type, quint16 pos);
};

#endif // MODIFYCO_H
