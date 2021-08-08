#ifndef TIMED_RELAX_H
#define TIMED_RELAX_H

#include <QWidget>
#include "transparent.h"

QT_BEGIN_NAMESPACE
namespace Ui { class timed_relax; }
QT_END_NAMESPACE

class timed_relax : public QWidget
{
    Q_OBJECT
    
public:
    timed_relax(QWidget *parent = nullptr);
    ~timed_relax();
    void ui_start(bool flag);
    void ui_create_tip(bool flag);

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void on_timer_start_clicked();
    
private:
    QWidget*        m_tip;              /* 提示窗口 */
    uint32_t        m_timer_cnt;        /* 计时 */
    uint32_t        m_timer_minute;     /* 定时分钟 */
    bool            m_timer_repeat;     /* 0单次 1重复 */
    bool            m_timer_start;      /* 0停止 1开始 */

private:
    Ui::timed_relax *ui;
};
#endif // TIMED_RELAX_H
