#ifndef BTSRSTARTSTUDY_H
#define BTSRSTARTSTUDY_H

#include <QDialog>
#include <QTimer>
#include "myheader.h"

namespace Ui {
class BTSRStartStudy;
}

class BTSRStartStudy : public QDialog
{
    Q_OBJECT

public:
    explicit BTSRStartStudy(QWidget *parent = 0);
    ~BTSRStartStudy();

private slots:
    void on_pushButton_study_clicked();

    void on_return_btn_clicked();

    void InputEvent();//处理键盘的输入
    void on_pushButton_startplustest_clicked();

    void on_pushButton_plusstudytest_clicked();

    void on_pushButton_stopplustest_clicked();

    void on_return_autotest_clicked();

private:
    Ui::BTSRStartStudy *ui;
    QTimer *keyboard_timer;
    bool autoTest;
    int autoTestCount;
    int count;
};

#endif // BTSRSTARTSTUDY_H
