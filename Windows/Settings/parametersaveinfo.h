#ifndef PARAMETERSAVEINFO_H
#define PARAMETERSAVEINFO_H

#include <QDialog>
#include <QTimer>
namespace Ui {
class ParameterSaveInfo;
}

class ParameterSaveInfo : public QDialog
{
    Q_OBJECT
    
public:
    explicit ParameterSaveInfo(QWidget *parent = 0);
    ~ParameterSaveInfo();
    void setShowText(QString str);
    
private:
    Ui::ParameterSaveInfo *ui;

public slots:

};

#endif // PARAMETERSAVEINFO_H
