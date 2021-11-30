#ifndef MENU_H
#define MENU_H

#include <QDialog>
#include <QTimer>
#include "Windows/Settings/settingmain.h"
#include "Windows/Testings/testingmain.h"
#include "Windows/FileOperations/fileoperations.h"
#include "Windows/SystemMaintenance/systemmaintence.h"
#include "QCom/qextserialport.h"
#include "myheader.h"

namespace Ui {
class Menu;
}

class Menu : public QDialog
{
    Q_OBJECT
public:
    explicit Menu(QWidget *parent = 0);
    ~Menu();

private slots:
     void update_slot();
     void on_setting_pb_clicked();
     void on_testing_pb_clicked();
     void on_fileoperation_pb_clicked();
     void on_systemmantence_pb_clicked();
     void on_patternwoven_pb_clicked();
     void on_codesetting_pb_clicked();
     void on_faultdetection_pb_clicked();

private:
    Ui::Menu *ui;
    QTimer *timer_menu;
};

#endif // MENU_H
