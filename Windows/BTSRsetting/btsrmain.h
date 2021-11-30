#ifndef BTSRMAIN_H
#define BTSRMAIN_H

#include <QDialog>
#include <QTimer>
#include "myheader.h"

namespace Ui {
class BTSRMain;
}

class BTSRMain : public QDialog
{
    Q_OBJECT

public:
    explicit BTSRMain(QWidget *parent = 0);
    ~BTSRMain();

private slots:
    void on_toolButton_setid_clicked();

    void on_toolButton_yarn_parameter_clicked();

    void on_toolButton__study_clicked();

    void on_toolButton_sensor_parameter_clicked();

    void on_toolButton_function_set_clicked();

    void on_toolButton_sensor_disable_clicked();

    void on_return_btn_clicked();

    void InputEvent();//处理键盘的输入

private:
    Ui::BTSRMain *ui;
    QTimer *keyboard_timer;
};

#endif // BTSRMAIN_H
