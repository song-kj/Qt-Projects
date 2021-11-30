#ifndef CLICKEDQLABEL
#define CLICKEDQLABEL
#include <QLabel>
#include <QMouseEvent>
#include <QAbstractButton>

class ClickedQLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickedQLabel(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    bool isPress;
 signals:
    void clicked();
public slots:

};

#endif // CLICKEDQLABEL



