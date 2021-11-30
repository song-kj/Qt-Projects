#ifndef UPGRADEFEEDBACK_H
#define UPGRADEFEEDBACK_H

#include <QDialog>

namespace Ui {
class UpgradeFeedback;
}

class UpgradeFeedback : public QDialog
{
    Q_OBJECT

public:
    explicit UpgradeFeedback(int type,QWidget *parent = 0);
    ~UpgradeFeedback();

private slots:
    void on_pushButton_clicked();


public slots:
    void feedback(int index, int b);

private:
    Ui::UpgradeFeedback *ui;
    QString proName;
    int count;
    int maxfeeder;
};

#endif // UPGRADEFEEDBACK_H
