#ifndef TEXTTICKER_H
#define TEXTTICKER_H

#include <QLabel>

class TextTicker : public QLabel
{
    Q_OBJECT
public:
    explicit TextTicker(QWidget *parent = 0);
    void setShowText(QString text);

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void updateIndex();

private:
    int m_charWidth;
    int m_curIndex;
    QString m_showText;
    QString m_text;
};

#endif // TEXTTICKER_H
