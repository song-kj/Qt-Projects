
#include "Tool/OverloadLabelAndLineEdit/clickedqlabel.h"
ClickedQLabel::ClickedQLabel(QWidget *parent) :
    QLabel(parent),isPress(false)
{
}

void ClickedQLabel::mousePressEvent(QMouseEvent *)
{
    isPress = true;
}

void ClickedQLabel::mouseReleaseEvent(QMouseEvent *)
{
    if (isPress)
    {
        emit clicked();
    }
    isPress = false;
}
