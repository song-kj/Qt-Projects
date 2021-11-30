#ifndef VERSIONQUERY_H
#define VERSIONQUERY_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class VersionQuery;
}

struct versionType{
    quint8 year;
    quint8 month;
    quint8 day;
    quint8 num;
};

class VersionQuery : public QDialog
{
    Q_OBJECT

public:
    explicit VersionQuery(QWidget *parent = 0);
    ~VersionQuery();

private slots:
    void on_btn_query_clicked();

    void on_btn_back_clicked();

    void timeout();
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::VersionQuery *ui;
    QTimer *timer1;
    versionType versions[16];
    QStringList borderNames;
    bool querying;
    int timecount;
    int version_type;
    int borderNum;
    void initialForm();
    void readVersions();
    void versionShow();
};

#endif // VERSIONQUERY_H
