#ifndef CODESETTINGMAIN_H
#define CODESETTINGMAIN_H

#include <QDialog>
#include "myheader.h"

namespace Ui {
class CodesettingMain;
}

class CodesettingMain : public QDialog
{
    Q_OBJECT

public:
    explicit CodesettingMain(QWidget *parent = 0);
    ~CodesettingMain();

private slots:
    void on_btn_selcode_clicked();

    void on_btn_valvecode_clicked();

    void on_btn_motorcode_clicked();

    void on_btn_back_clicked();

private:
    Ui::CodesettingMain *ui;
};

#endif // CODESETTINGMAIN_H
