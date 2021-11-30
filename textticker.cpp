#include "textticker.h"
#include <QPainter>
#include <QTimer>
#include <QDebug>

TextTicker::TextTicker(QWidget *parent) : QLabel(parent)
{
    m_showText=="";
    m_curIndex = 0;//当前角码
    m_charWidth = fontMetrics().width("a");//每个字符的宽度

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateIndex()));
    timer->start(200);
}

void TextTicker::setShowText(QString text)
{
    m_text=text;//显示的文字
    m_showText=QString("%1     %2").arg(m_text).arg(m_text);
}

void TextTicker::paintEvent(QPaintEvent *event)
{
    if(m_showText!="")
    {
        QPainter painter(this);
        painter.drawText(0, 35, m_showText.mid(m_curIndex));
    }
}

void TextTicker::updateIndex()
{
    m_curIndex++;
    if(m_curIndex>=m_text.length()+5)
        m_curIndex=0;
    update();
}
