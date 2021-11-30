#ifndef DIALOGSYSSETIING_H
#define DIALOGSYSSETIING_H
#include "hirepurchase.h"
#include <QDialog>
#include <QTimer>
#include <QFileSystemWatcher>
#include <QDateTime>
#include "myheader.h"

namespace Ui {
class DialogSysSetiing;
}

class DialogSysSetiing : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSysSetiing(QWidget *parent = 0);
    ~DialogSysSetiing();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void update_slot();
    void getUSB(QString);

    void on_pushButton_clear_clicked();

private:
    Ui::DialogSysSetiing *ui;
    void showinfo_USB();
    Hirepurchase *hire;
    void showStartTime(QString);
    void showEndingTime(QString);
    QTimer *pTimer;
    bool isfirstEnter;
    QFileSystemWatcher *watcher;
    void initialForm();
    int clickCount;
    QDateTime clickFirstTime;
};

#endif // DIALOGSYSSETIING_H
