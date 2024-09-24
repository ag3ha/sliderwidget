#include "CustomLabel.h"

bool CustomLabel::pending = false;  // 静态变量初始化

CustomLabel::CustomLabel(QWidget *parent): QLabel(parent)
{

}

void CustomLabel::enterEvent(QEnterEvent *event)
{
    if(isScoll == false)
    //发送悬浮状态开始
        emit hoverSignal(true,this);  // 发送悬浮开始信号
        setCursor(Qt::PointingHandCursor); // 设置鼠标样式为手指
    // 保证 QLabel 的其他行为不受影响
    QLabel::enterEvent(event);
}

void CustomLabel::leaveEvent(QEvent *event)
{
    //发送悬浮状态结束？
    if(isScoll == false)
        emit hoverSignal(false,this);  // 发送悬浮结束信号
        setCursor(Qt::ArrowCursor); // 恢复默认鼠标样式
    QLabel::leaveEvent(event);
}


void CustomLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        qDebug() << "Label clicked:" << text(); // 点击时输出信息
        setScoll(true);
        emit labelClicked(this); // 发射自定义信号
    }
    QLabel::mousePressEvent(event);
}
