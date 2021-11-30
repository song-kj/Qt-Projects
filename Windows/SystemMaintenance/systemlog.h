#ifndef SYSTEMLOG_H
#define SYSTEMLOG_H

#include <QDialog>
#include <QList>
#include <QString>
#include <QScrollBar>
#include <QTimer>
#include "myheader.h"

namespace Ui {
class SystemLog;
}

class SystemLog : public QDialog
{
    Q_OBJECT

public:
    explicit SystemLog(QWidget *parent = 0);
    ~SystemLog();

private slots:

    void update_slot();

    void on_back_Mot_pb_clicked();

    void on_back_Mot_pb_2_clicked();

    void on_back_copy_clicked();

private:
    Ui::SystemLog *ui;
    QTimer *pTimer;
    void initialForm();
    void readFromFile(QString filename);
};

#endif // SYSTEMLOG_H
