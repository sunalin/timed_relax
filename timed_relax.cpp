#include "timed_relax.h"
#include "ui_timed_relax.h"
#include <QDebug>

timed_relax::timed_relax(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::timed_relax)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);//new的窗口 close()时自动回收资源
    Qt::WindowFlags flags = Qt::Widget;
    flags |= Qt::FramelessWindowHint;           /* 窗口无标题栏 */
    this->setWindowFlags(flags);
    connect(ui->timer_close, static_cast<void (QAbstractButton::*)(bool)>(&QAbstractButton::clicked),
            this,            [=](bool){this->close();});  /* 按钮整个窗口 */

    startTimer(1000);   // 1-second timer
    m_tip = NULL;
    m_timer_start = false;

    on_timer_start_clicked();               /* 打开软件后立即开始计时 */
}

timed_relax::~timed_relax()
{
    if (m_tip) {
        m_tip->close();
        m_tip = NULL;
    }
    delete ui;
}

void timed_relax::ui_start(bool flag)
{
    if (flag == true) {
        ui->tip->setEnabled(false);
        ui->timer_minute->setEnabled(false);
        ui->timer_repeat->setEnabled(false);
        ui->timer_start->setText("停止");
    } else {
        ui->tip->setEnabled(true);
        ui->timer_minute->setEnabled(true);
        ui->timer_repeat->setEnabled(true);
        ui->timer_start->setText("计时");
    }
}

void timed_relax::ui_create_tip(bool flag)
{
    if (m_tip) {
        m_tip->close();
    }
    m_tip = NULL;
    if (flag) {
        m_tip = new transparent(ui->tip_top->isChecked(),
                                !ui->tip_break->isChecked(),
                                ui->tip_showtime->isChecked(),
                                ui->tip_timeout->currentText().toUInt() * 60);
        connect(m_tip, static_cast<void (QObject::*)(QObject*)>(&QObject::destroyed),
                this,  [=](QObject* obj){if (obj == m_tip) m_tip = NULL; qDebug() << "提示窗口销毁";});
        m_tip->show();
    }
}

void timed_relax::timerEvent(QTimerEvent*)
{
    if (m_timer_start) {
        if (++m_timer_cnt >= m_timer_minute) {
            qDebug() << "时间到";
            ui_create_tip(m_timer_start);

            if (m_timer_repeat == true) {
                m_timer_cnt = 0;
            } else {
                m_timer_start = false;
            }
            ui_start(m_timer_start);
        }
        ui->timer_text->setText(QTime(0, 0, 0, 0).addSecs(m_timer_minute - m_timer_cnt).toString("剩余：mm:ss"));
    }
}

void timed_relax::on_timer_start_clicked()
{
    if (m_timer_start == true) {
        m_timer_start = false;
        ui_create_tip(m_timer_start);
    } else {
        m_timer_cnt = 0;
        m_timer_minute = ui->timer_minute->currentText().toUInt();
        m_timer_repeat = ui->timer_repeat->isChecked();
        if (m_timer_minute == 0)
            m_timer_minute = 1;
        m_timer_minute *= 60;
        m_timer_start = true;
    }

    ui_start(m_timer_start);
}
