#ifndef WARNINGSETTING_H
#define WARNINGSETTING_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QFile>
#include <QTimer>

namespace Ui {
class WarningSetting;
}

class WarningSetting : public QDialog
{
    Q_OBJECT

public:
    explicit WarningSetting(QWidget *parent = 0);
    ~WarningSetting();

private slots:
    void on_btn_back_clicked();

    void on_btn_ok_clicked();
    void timeout();

    void on_btn_all_clicked();

private:
    Ui::WarningSetting *ui;
    QStringList header;
    quint8 warnstate[10];
    bool startsend;
    bool allset;
    int rowCount;
    int timecount;
    QTimer *timer1;
};

#endif // WARNINGSETTING_H
