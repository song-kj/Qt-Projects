#ifndef TESTINGMAIN_H
#define TESTINGMAIN_H

#include <QDialog>
#include <QTimer>
#include "Windows/Testings/actuatortesting.h"
#include "Windows/Testings/airvalvetesting.h"
#include "Windows/Testings/inputtesting.h"
#include "Windows/Testings/outputtesting.h"
#include "Windows/Testings/steppermotortesting.h"
#include "Tool/OverloadLabelAndLineEdit/clickedqlabel.h"
#include "myheader.h"

namespace Ui {
class TestingMain;
}

class TestingMain : public QDialog
{
    Q_OBJECT

public:
    explicit TestingMain(QWidget *parent = 0);
    ~TestingMain();

private slots:
    void update_slot();

    void on_toolButton_airvalve_clicked();

    void on_toolButton_actuator_clicked();

    void on_toolButton_stepmotor_clicked();

    void on_toolButton_input_clicked();

    void on_toolButton_output_clicked();

    void on_toolButton_back_clicked();

private:
    Ui::TestingMain *ui;
    QTimer *timer_testmain;
};

#endif // TESTINGMAIN_H
