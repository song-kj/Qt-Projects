#ifndef INPUTTESTING_H
#define INPUTTESTING_H
#include <QDialog>
#include <QTimer>
#include <QDebug>
#include <QToolButton>
#include "Tool/OverloadLabelAndLineEdit/clickedqlabel.h"
#include "myheader.h"
#include <QMessageBox>
namespace Ui
{
class InputTesting;
}

class InputTesting : public QDialog
{
    Q_OBJECT
public:
    explicit InputTesting(QWidget *parent = 0);
    ~InputTesting();

private slots:
    void update_input_slot();
    void on_pushButton_back_clicked();

private:
    Ui::InputTesting *ui;
    QLabel *label_text[32];
    QToolButton *tlb_state[32];
    quint8 id[32];
    QTimer *timer_input;
    quint8 value_state[4];
    void initialForm();
};
#endif // INPUTTESTING_H
