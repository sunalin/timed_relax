#ifndef TRANSPARENT_H
#define TRANSPARENT_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPainter>
#include <QPushButton>
#include <QLCDNumber>
#include <QTime>
#include <QDebug>
#include <QEvent>


#define BACKGROUND_CLICK_SECS       5   /* 每次切换窗口使背景n秒内可击 */

class transparent : public QWidget
{
    Q_OBJECT
    
public:
    explicit transparent(bool top = false,
                         bool enbreak = true,
                         bool showtime = true,
                         uint32_t timeout_close_win = 0,
                         QWidget *parent = nullptr)
        : QWidget(parent)
        , m_timeout_close_win(timeout_close_win)
        , m_timeout_enbale_closebtn(5)
        , m_background_click(BACKGROUND_CLICK_SECS)
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
        m_closebtn->setFixedSize(120, 60);
        m_closebtn->setStyleSheet("QPushButton{font-family:'Microsoft YaHei';font-size:33px;color:#666666;}"
                                  "QPushButton{border:1px groove gray;border-radius:10px;padding:2px 4px;}");
        m_closebtn->setEnabled(false);
        m_closebtn->setVisible(enbreak);

        /* 时间lcd */
        m_lcd = new QLCDNumber;
        m_lcd->setFrameShape(QFrame::NoFrame);
        m_lcd->setFrameShadow(QFrame::Plain);
        m_lcd->setSegmentStyle(QLCDNumber::Flat);
        m_lcd->setFixedSize(90, 25);
        m_lcd->setStyleSheet("QLCDNumber{font-family:'Microsoft YaHei';font-size:120px;color:#666666;}");
        m_lcd->display(" ");
        m_lcd->setVisible(showtime);

        /* 布局 */
        QHBoxLayout* h_btn = new QHBoxLayout;
        h_btn->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
        h_btn->addWidget(m_closebtn);
        h_btn->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
        h_btn->setContentsMargins(0, 0, 0, 0);
        h_btn->setSpacing(0);

        QHBoxLayout* h_lcd = new QHBoxLayout;
        h_lcd->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
        h_lcd->addWidget(m_lcd);
        h_lcd->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
        h_lcd->setContentsMargins(0, 0, 0, 0);
        h_lcd->setSpacing(0);

        QVBoxLayout* vbox = new QVBoxLayout;
        vbox->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
        vbox->addLayout(h_btn);
        vbox->addLayout(h_lcd);
        vbox->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
        vbox->setContentsMargins(0, 0, 0, 0);
        vbox->setSpacing(0);
        this->setLayout(vbox);


        this->installEventFilter(this);
        this->showFullScreen();
        this->startTimer(1000);   // 1-second timer
    }

    ~transparent()
    {

    }

protected:
    bool eventFilter(QObject* obj, QEvent* event)
    {
        if (obj == this) {
            if (event->type() == QEvent::WindowActivate || event->type() == QEvent::WindowDeactivate) {
                /* 窗口活动 */
                /*if (m_background_click == 0)*/ {
                    m_background_click = BACKGROUND_CLICK_SECS;
                }
                this->update();
            }
        }  
        return QWidget::eventFilter(obj, event);
    }

    void paintEvent(QPaintEvent*)
    {
        QPainter painter(this);
        painter.fillRect(this->frameGeometry(), QColor(0, 0, 0, !m_background_click));  //QColor最后一个参数80代表背景的透明度
        painter.fillRect(m_closebtn->frameGeometry(), QColor(0, 0, 0, 1));  //QColor最后一个参数80代表背景的透明度
        painter.fillRect(m_lcd->frameGeometry(), QColor(0, 0, 0, 1));  //QColor最后一个参数80代表背景的透明度
    }

    void timerEvent(QTimerEvent*)
    {
        if (!m_timeout_close_win || (m_timeout_close_win && --m_timeout_close_win == 0)) {
            this->close();                  /* 超时关闭整个窗口 */
            return;
        }

        if (m_timeout_enbale_closebtn && --m_timeout_enbale_closebtn == 0) {
            m_closebtn->setEnabled(true);   /* 超时后关闭按钮才能按 */
        }

        if (m_lcd->isVisible()) {
            m_lcd->display(QTime(0, 0, 0, 0).addSecs(m_timeout_close_win).toString("mm:ss"));
        }
        
        if (m_background_click) {
            if (--m_background_click == 0) {
                //this->activateWindow();
            }
            this->update();
        }
    }

private:
    uint32_t        m_timeout_close_win;        /* (秒)超时自动关闭窗口, 初始为0表示不自动关闭 */
    uint32_t        m_timeout_enbale_closebtn;  /* (秒)超时后关闭按钮才能按 */
    uint32_t        m_background_click;         /* 0: 在透明背景下无法点击背后窗口 */
    QPushButton*    m_closebtn;                 /* 关闭按钮 */
    QLCDNumber*     m_lcd;                      /* 时间lcd */
};

#endif // TRANSPARENT_H
