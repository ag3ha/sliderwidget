#include "CustomScrollArea.h"

static bool newAnimationRequested = false;
static int newTargetValue = 0;
static int peak = 2;
CustomScrollArea::CustomScrollArea(QWidget *parent)
    : QScrollArea(parent),timer(new QTimer(this)),scrollBar(verticalScrollBar())
{
    connect(timer,&QTimer::timeout,[=](){onScrollTimeout();});
}

void CustomScrollArea::setScorllV(int end)
{
    int start = scrollBar->value();
    animation = new QPropertyAnimation(scrollBar, "value");
    // 如果动画正在运行，则标记请求，并保存新目标值
    if (animation->state() == QAbstractAnimation::Running) {
        newAnimationRequested = true;
        newTargetValue = end;
        return;
    }
    animation->setDuration(100); // Duration in milliseconds
    animation->setStartValue(start);
    animation->setEndValue(end);
    previousValue = start; // Update previous value
    // 开始新的动画
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void CustomScrollArea::wheelEvent(QWheelEvent *event)
{
    int currentValue = scrollBar->value();

    if (!scrolling) {  // 标记一次滚动的开始
        firstValue = currentValue;
        scrolling = true;
        emit isScrolling(scrolling);
    }

    previousValue = currentValue;

    timer->start(50);  // 每次滚轮事件触发时重置定时器
    QScrollArea::wheelEvent(event);

    QTimer::singleShot(0,[=](){
        QScrollBar *scrollBar = verticalScrollBar();
        int currentValue = scrollBar->value();
        if (currentValue > previousValue) {
            if(currentValue < scrollBar->maximum()) {//down
                emit sliderMove(-1);
            }
        } else {
            if(currentValue > 0) {//up
                emit sliderMove(1);
            }
        }
    });
}

void CustomScrollArea::startScrollBarAnimation()
{
    int index = abs(previousValue - firstValue)/70 + 1;

    scrollBar = verticalScrollBar();
    int currentValue = scrollBar->value();
    int targetValue;

    if (currentValue > previousValue) {
        targetValue = ((currentValue / 70) + 1) * 70;
        if(currentValue < scrollBar->maximum()) {
            offset -= index;
            emit animationStart(-1);
        } else {
            return;
        }
    } else {
        targetValue = ((currentValue / 70)) * 70;
        if(currentValue > 0) {
            offset += index;
            emit animationStart(1);
        } else {
            return;
        }
    }

    if (currentValue != targetValue)
    {
        peak = 2 - offset; // 峰值的索引
        targetValue = 70 * (peak-2);
        setScorllV(targetValue);
    }

}

// 定时器超时函数，判断滚动结束
void CustomScrollArea::onScrollTimeout()
{
    if (!scrolling)
        return;

    scrolling = false;  // 重置滚动标志
    emit isScrolling(scrolling);
    startScrollBarAnimation();  // 滚动结束后启动动画
}
