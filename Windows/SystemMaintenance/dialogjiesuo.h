#ifndef DIALOGJIESUO_H
#define DIALOGJIESUO_H
#include "hirepurchase.h"
#include <QDialog>
#include <QTimer>
#include <QFileSystemWatcher>
#include "myheader.h"

namespace Ui {
class DialogJiesuo;
}

class DialogJiesuo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogJiesuo(QWidget *parent = 0);
    ~DialogJiesuo();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void getString(QString);

    void on_autoJiesuo_btn_clicked();

    void update_slot();

    void getUSB(QString);

    void on_btn_back_clicked();

    void on_tabWidget_currentChanged(int index);

private:
    Ui::DialogJiesuo *ui;
    Hirepurchase *hire;
    void showinfo_USB();
    QTimer *pTimer;
    quint8 idInt[10];
    QFileSystemWatcher *watcher;
    bool isfirstEnter;
    void createRandCode();
};

#endif // DIALOGJIESUO_H
