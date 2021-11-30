#ifndef ERRORCODE_H
#define ERRORCODE_H
#include <QMessageBox>

class ErrorCode : public QMessageBox
{
public:
    ErrorCode();
    ~ErrorCode();
    void DisErrorMsg(Icon icon,const QString &msg);
private:

};

#endif // ERRORCODE_H
