#ifndef STITCHADJUST_H
#define STITCHADJUST_H

#include <QDialog>
#include <QLabel>
#include <QToolButton>
#include <QTimer>

namespace Ui {
class StitchAdjust;
}

class StitchAdjust : public QDialog
{
    Q_OBJECT

public:
    explicit StitchAdjust(QWidget *parent = 0);
    ~StitchAdjust();
    void setType(int t);

private slots:
    void on_btn_back_clicked();
    void motorclicked();

    void on_btn_add_clicked();

    void on_btn_sub_clicked();

    void on_btn_setall_clicked();

    void on_btn_save_clicked();

    void timeout();

    void saveok();
    void on_btn_chainstitch_clicked();

private:
    Ui::StitchAdjust *ui;
    QLabel *label_name[8];
    QLabel *label_adjust[8];
    QToolButton *btn_motors[8];
    qint16 motor_value[8];
    qint16 motor_adjust[8];
    QTimer *timer1;
    bool setall;
    bool modifyed;
    int type;
    int cur_index;
    void initialForm();
    void downloadmotor();
    void numberinput(QString num);
};

#endif // STITCHADJUST_H
