#ifndef SOFTWEAR_H
#define SOFTWEAR_H

#include <QDialog>

namespace Ui {
class Softwear;
}

class Softwear : public QDialog
{
    Q_OBJECT

public:
    explicit Softwear(QWidget *parent = 0);
    ~Softwear();
private slots:

    void dealWithClick();
    void on_A_btn_2_clicked();

    void on_Shift_btn_clicked();

    void on_Enter_btn_clicked();

    void on_Esc_btn_clicked();

    void on_Back_btn_clicked();

signals:
    void setNumber(QString);
private:
    Ui::Softwear *ui;
    QString number;
    QString showNumber;
    QPushButton* button;
    bool ABool;
    bool ShiftBool;

    bool AEnterOrNot;
    bool ShiftEnterOrNot;
    QString AaTransform(QChar);
    QString StrTransform(QString);
};

#endif // SOFTWEAR_H
