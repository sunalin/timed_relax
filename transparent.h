#ifndef TRANSPARENT_H
#define TRANSPARENT_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPainter>
#include <QPushButton>


class transparent : public QWidget
{
    Q_OBJECT
    
public:
    explicit transparent(bool top = false, uint32_t timeout_close_win = 0, QWidget *parent = nullptr)
        : QWidget(parent)
        , m_timeout_close_win(timeout_close_win)
        , m_timeout_enbale_closebtn(5)
    {
        this->setAttribute(Qt::WA_DeleteOnClose);// 通过new出来的窗口 close()时自动回收资源
        //this->setWindowModality(Qt::ApplicationModal);
        Qt::WindowFlags flags = Qt::Widget;
        flags |= Qt::FramelessWindowHint;           /* 窗口无标题栏 */
        if (top) flags |= Qt::WindowStaysOnTopHint; /* 顶层窗口 */
        this->setWindowFlags(flags);
        //this->setWindowOpacity(0.1);    // 半透明
        //this->setFixedSize(350, 120);
        this->setAttribute(Qt::WA_TranslucentBackground, true);
        //QPalette pal = palette();
        //pal.setColor(QPalette::Background, QColor(0, 0, 0, 1));
        //this->setPalette(pal);

        /* 按钮属性 */
        m_closebtn = new QPushButton("break");
        connect(m_closebtn, static_cast<void (QAbstractButton::*)(bool)>(&QAbstractButton::clicked),
                this,       [=](bool){this->close();});  /* 按钮关闭整个窗口 */
        m_closebtn->setFixedSize(150, 70);
        m_closebtn->setStyleSheet("QPushButton{font-family:'Microsoft YaHei';font-size:40px;color:#666666;}"
                                  "QPushButton{border:2px groove gray;border-radius:10px;padding:2px 4px;}");
        m_closebtn->setEnabled(false);

        /* 布局 */
        QHBoxLayout* hbox = new QHBoxLayout;
        hbox->addItem(new QSpacerItem(97, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
        hbox->addWidget(m_closebtn);
        hbox->addItem(new QSpacerItem(97, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
        this->setLayout(hbox);


        this->showFullScreen();
        this->startTimer(1000);   // 1-second timer
    }

    ~transparent()
    {

    }

protected:
    void paintEvent(QPaintEvent*)
    {
        QPainter painter(this);
        painter.fillRect(this->rect(), QColor(0, 0, 0, 1));  //QColor最后一个参数80代表背景的透明度
    }

    void timerEvent(QTimerEvent*)
    {
        if (m_timeout_close_win && --m_timeout_close_win == 0) {
            this->close();                  /* 超时关闭整个窗口 */
        }

        if (m_timeout_enbale_closebtn && --m_timeout_enbale_closebtn == 0) {
            m_closebtn->setEnabled(true);   /* 超时后关闭按钮才能按 */
        }
    }

private:
    uint32_t        m_timeout_close_win;        /* (秒)超时自动关闭窗口, 初始为0表示不自动关闭 */
    uint32_t        m_timeout_enbale_closebtn;  /* (秒)超时后关闭按钮才能按 */
    QPushButton*    m_closebtn;                 /* 关闭按钮 */
};

#endif // TRANSPARENT_H
