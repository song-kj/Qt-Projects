#ifndef ACTUATORTESTING_H
#define ACTUATORTESTING_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QFrame>
#include <QTimer>
#include <QMessageBox>
#include <QStyleFactory>
#include "Tool/OverloadLabelAndLineEdit/clickedqlabel.h"
#include "myheader.h"
namespace Ui {
class ActuatorTesting;
}

class ActuatorTesting : public QDialog
{
    Q_OBJECT

public:
    explicit ActuatorTesting(QWidget *parent = 0);
    ~ActuatorTesting();
    void show_actuator(int,int);
    int SingleUpDown();
    void WholeUpDown();
    void showRED(int,int);
    void showGUI(int,int);
    void touchShowTime_ActuatorTesting();
private slots:
     void on_previouspage_Act_pb_clicked();
     void on_nextpage_Act_pb_clicked();
     void showframe();

     void singleupdown_slot();
     void wholeupdown_slot();
     void singleinterval_slot();
     void wholeinterval_slot();
     void manualtest_slot();
     void backfrommanul_slot();
     void back_slot();
     void circulatetest_slot();
     void debugtest_slot();
     void plus_slot();
     void minus_slot();
     void backfromcirculation_slot();
     void backfromdebug_slot();
     void singledebug_slot();
     void wholedebug_slot();
     void debuglist_slot();
     void debugreset_slot();
     void selflearning_slot();
     void singlematuring_slot();
     void wholematuring_slot();
     void singlecirculation_slot();
     void wholecirculation_slot();
    void FrameSelectedFirst_slot();
     void modtest_Act_pb_clicked();
    void update_slot();

private:
    Ui::ActuatorTesting *ui;
    QPushButton *pin[16][16];
    QPushButton *ping;
    QLabel *ActuatotRoad[16];
    int PreRoad;
    int Road;
    ClickedQLabel *frame[16];
    QPushButton *framecover[16];
    QPushButton *click;
    int k;
    int mark;
    int pre_i;
    int istring;
    bool UpDown_single;
    int Sum_single;
    bool UpDown_whole;
    int Sum_whole;
    int Sum_singinterval;
    bool interval_single;
    int Sum_wholeinterval;
    bool interval_whole;
    bool updown_whole;

    QPushButton*manual[8];
    QPushButton*circulation[6];
    QPushButton* debug[6];
    QPushButton* manualtest_Act_pb;   //????????????
    QPushButton* circulatetest_Act_pb;//????????????
    QPushButton* debugtest_Act_pb;    //????????????
    QPushButton* empty1_Act_pb;       //??????1
    QPushButton* empty2_Act_pb;       //??????2
    QPushButton* plus_Act_pb;         //??????
    QPushButton* minus_Act_pb;        //??????
    QPushButton* back_Act_pb;         //??????
    QPushButton* modtest_Act_pb;      //????????????

    int sum;
    int sum_single_Act;
    int sum_interval_Act;
    int sum_whole_Act;
    int sum_interval_whole_Act;
    int ManualAndDebugSwitch;//??????????????????????????????????????????????????????
    QTimer *timer_act;
    int startSelectRoad;
    int whichWidget;
    int mod; //??????
    bool singleOld;
    bool whloeOld;
    bool wholeCir;
    bool singleCir;
    QLabel *touchShow_ActuatorTesting;
    bool touchStart_ActuatorTesting;

    int single_updown_or_single_debug;
    int whole_updown_or_whole_debug;
};

#endif // ACTUATORTESTING_H
