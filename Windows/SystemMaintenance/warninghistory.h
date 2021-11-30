#ifndef WARNINGHISTORY_H
#define WARNINGHISTORY_H

#include <QDialog>
#include <QDate>
#include <QDir>
#include "myheader.h"
#include <QTimer>

namespace Ui {
class WarningHistory;
}

class WarningHistory : public QDialog
{
    Q_OBJECT

public:
    explicit WarningHistory(QWidget *parent = 0);
    ~WarningHistory();

private slots:
    void timeout();

    void on_back_Mot_pb_clicked();

    void on_dateEdit_dateChanged(const QDate &date);

    void on_btn_lastday_clicked();

    void on_btn_nextday_clicked();

    void on_deletethisday_clicked();

    void on_deleteall_clicked();

    void on_btn_today_clicked();

    void on_btn_pageup_clicked();

    void on_btn_pagedown_clicked();

private:
    Ui::WarningHistory *ui;
    int maxRowCount;
    int totalPages;
    int curPage;
    QTimer *timer1;
    void initialForm();
    QString getWarningName(quint16 addr,quint8 bite);
    void readFromFile(QString filename);
};

#endif // WARNINGHISTORY_H
