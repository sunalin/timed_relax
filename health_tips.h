#ifndef HEALTH_TIPS_H
#define HEALTH_TIPS_H

#include <QWidget>
#include "ui_health_tips.h"

#include <QMouseEvent>
#include <QPainter>
#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif

namespace Ui {
class health_tips;
}

class health_tips : public QWidget
{
    Q_OBJECT
    
public:
    explicit health_tips(QWidget *parent = nullptr) :
        QWidget(parent),
        ui(new Ui::health_tips)
    {
        ui->setupUi(this);

        ui->text->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:20pt;\">Feel it and accept it.<br>Accept all of my life.</span></p></body></html>");
        this->setAttribute(Qt::WA_DeleteOnClose);// 通过new出来的窗口 close()时自动回收资源
        //this->setWindowModality(Qt::ApplicationModal);
        this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);   // 顶层窗口无标题
        //this->setWindowOpacity(1);    // 半透明
        this->setFixedSize(350, 120);
        setAttribute(Qt::WA_TranslucentBackground);
        m_time_cnt = 5 * 60;
        startTimer(1000);   // 1-second timer
    }
    ~health_tips()
    {
        delete ui;
    }
    
protected:
    void timerEvent(QTimerEvent *event)
    {
        Q_UNUSED(event);
        if (m_time_cnt)
        {
            if (--m_time_cnt == 0)
                this->close();
            ui->num->setText(QString("%1:%2")
                             .arg(m_time_cnt/60, 2, 10, QLatin1Char('0'))
                             .arg(m_time_cnt%60, 2, 10, QLatin1Char('0')));
        }
    }
    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
        painter.fillRect(this->rect(), QColor(0, 0, 0, 75)); /* 设置透明颜色 */
    }
    void mousePressEvent(QMouseEvent *event)
    {
        #ifdef Q_OS_WIN
        if (ReleaseCapture())
               SendMessage(HWND(winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
           event->ignore();
        #else
        // 鼠标相对于窗体的位置 event->globalPos() - this->pos()
        if (event->button() == Qt::LeftButton)
        {
            m_bPressed = true;
            m_point = event->pos();
        }
        #endif
    }
    void mouseMoveEvent(QMouseEvent *event)
    {
        // 若鼠标左键被按下，移动窗体位置
        if (m_bPressed)
            move(event->pos() - m_point + pos());
    }
    void mouseReleaseEvent(QMouseEvent *event)
    {
        // 设置鼠标未被按下
        Q_UNUSED(event);

        m_bPressed = false;
    }
    
private:
    uint32_t m_time_cnt;    /* 倒计时 */
    bool m_bPressed;
    QPoint m_point;

private:
    Ui::health_tips *ui;
};

#endif // HEALTH_TIPS_H
