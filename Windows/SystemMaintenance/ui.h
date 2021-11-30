#ifndef UI_H
#define UI_H

#include <QDialog>
#include <QTimer>
#include <QLabel>
#include <QFileSystemWatcher>
#include <QDir>
#include <Tool/KeyBoard/keybd.h>
#include "myheader.h"

namespace Ui {
class UI;
}

class UI : public QDialog
{
    Q_OBJECT
    
public:
    explicit UI(QWidget *parent = 0);
    ~UI();
    
private slots:
    void on_OK_pb_clicked();

    void on_ESC_pb_clicked();
    void update_slot();
    void getUSB(QString);

private:
    Ui::UI *ui;
    QTimer *timer_ui;
    QString Path_usb;
    QString selectName;
    bool firstEnter;
    QFileSystemWatcher *watcher;
    void myseleep(unsigned int msec);
};

#endif // UI_H
