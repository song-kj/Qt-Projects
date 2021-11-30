#ifndef DIALOGCALCULATOR_H
#define DIALOGCALCULATOR_H

#include <QDialog>
#include <QPainter>
#include <qmath.h>
#include "myheader.h"

namespace Ui {
class DialogCalculator;
}

class DialogCalculator : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCalculator(QWidget *parent = 0);
    ~DialogCalculator();
    QString str;
    int maxValue;//参数最大值
    int minValue;//参数最小值
    void setFlag(bool b);

private slots:
    void on_btn_Back_clicked();
    
    void on_btn_Num7_clicked();

    void on_btn_Num8_clicked();

    void on_btn_Num9_clicked();

    void on_btn_Num4_clicked();

    void on_btn_Num5_clicked();

    void on_btn_Num6_clicked();

    void on_btn_Num1_clicked();

    void on_btn_Num2_clicked();

    void on_btn_Num3_clicked();

    void on_btn_Num0_clicked();

    void on_btn_Backspace_clicked();

    void on_btn_Sure_clicked();

    void on_btn_fuhao_clicked();

private:
    Ui::DialogCalculator *ui;
    bool firstOpen;
    bool zhengfu;

protected:

};

#endif // DIALOGCALCULATOR_H
