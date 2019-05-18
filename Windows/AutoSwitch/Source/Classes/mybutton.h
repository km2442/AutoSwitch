#pragma once
#define MYBUTTON_H

#include <QPainter>
#include <QPushButton>

using namespace std;

class MyButton : public QPushButton
{
public:
    explicit MyButton(QWidget* parent = nullptr);
    virtual ~MyButton();

    void setPixmap(const QPixmap& pixmap);

    virtual QSize sizeHint() const override;

protected:
    virtual void paintEvent(QPaintEvent* e) override;

private:
    QPixmap m_pixmap;
};

