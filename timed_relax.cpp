#include "timed_relax.h"
#include "ui_timed_relax.h"
#include <QDebug>

timed_relax::timed_relax(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::timed_relax)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);//new的窗口 close()时自动回收资源

    startTimer(1000);   // 1-second timer
    m_tip = NULL;
    m_timer_start = false;

    ui->timer_minute->setRange(0, 9999);
    ui->timer_minute->setSingleStep(10);
    ui->timer_minute->setValue(50);
}

timed_relax::~timed_relax()
{
    if (m_tip)
    {
        m_tip->close();
        m_tip = NULL;
    }
    delete ui;
}

void timed_relax::ui_start(bool flag)
{
    if (flag == true)
    {
        ui->timer_minute->setEnabled(false);
        ui->timer_repeat->setEnabled(false);
        ui->timer_start->setText("停止");
    }
    else
    {
        ui->timer_minute->setEnabled(true);
        ui->timer_repeat->setEnabled(true);
        ui->timer_start->setText("启动");
    }
}

void timed_relax::ui_create_tip(bool flag)
{
    if (m_tip)
        m_tip->close();
    m_tip = NULL;
    if (flag)
    {
        m_tip = new health_tips;
        if (m_tip)
        {
            connect(m_tip, static_cast<void (QObject::*)(QObject* obj)>(&QObject::destroyed), this,
                    [=](QObject* obj)
                    {
                        if (m_tip == (health_tips*)obj)
                        {
                            qDebug() << "子窗口销毁";
                            m_tip = NULL;
                        }
                    });
            m_tip->show();
        }
    }
}

void timed_relax::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    //qDebug() << "Timer ID:" << event->timerId();

    if (m_timer_start)
    {
        if (++m_timer_cnt >= m_timer_minute)
        {
            qDebug() << "时间到";
            ui_create_tip(m_timer_start);

            if (m_timer_repeat == true)
            {
                m_timer_cnt = 0;
            }
            else
            {
                m_timer_start = false;
            }
            ui_start(m_timer_start);
        }
        ui->timer_text->setText(QString("%1天 %2:%3:%4")
            .arg((m_timer_minute - m_timer_cnt)/(24*60*60))
            .arg((m_timer_minute - m_timer_cnt)%(24*60*60)/(1*60*60),    2, 10, QLatin1Char('0'))
            .arg((m_timer_minute - m_timer_cnt)%(24*60*60)%(1*60*60)/60, 2, 10, QLatin1Char('0'))
            .arg((m_timer_minute - m_timer_cnt)%(24*60*60)%(1*60*60)%60, 2, 10, QLatin1Char('0')));
    }
}

void timed_relax::on_timer_start_clicked()
{
    if (m_timer_start == true)
    {
        m_timer_start = false;
        ui_create_tip(m_timer_start);
    }
    else
    {
        m_timer_cnt = 0;
        m_timer_minute = ui->timer_minute->text().toUInt();
        m_timer_repeat = (ui->timer_repeat->currentText()=="重复") ? true : false;
        if (m_timer_minute == 0)
            m_timer_minute = 1;
        m_timer_minute *= 60;
        m_timer_start = true;
    }

    ui_start(m_timer_start);
}
