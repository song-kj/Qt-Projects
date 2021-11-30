#ifndef PATTERNOPERATION_H
#define PATTERNOPERATION_H

#include <QWidget>
#include <QTimer>
#include <QProgressBar>

namespace Ui {
class PatternOperation;
}

class PatternOperation : public QWidget
{
    Q_OBJECT
    
public:
    explicit PatternOperation(QWidget *parent = 0);
    ~PatternOperation();
    QProgressBar* proBar();

public slots:
    void getProgressData(int, int, const QString &);
    void update_slot();

private:
    Ui::PatternOperation *ui;
    int progressPreTime;
    QTimer *timer;
    QString patternName;
    int onlyFirstTimeUsing;
    int onlyFirstTimeUsingUCO;
};

#endif // PATTERNOPERATION_H
