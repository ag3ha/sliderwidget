#include "SiderListWidegt.h"
#include "ui_SiderListWidegt.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QColor>
#include <QPalette>
#include <QLayout>
#include "CustomLabel.h"
#include <QScrollArea>
#include "CustomScrollArea.h"


QStringList colors = {"red", "green", "blue", "yellow", "purple"};
QList<CustomLabel*> itemlist;

SiderListWidegt::SiderListWidegt(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget),previousLabel(nullptr)
{

    this->setFixedSize(300,380);
    scrollArea = new CustomScrollArea(this);
    containerWidget = new QWidget(); // 创建容器部件
    containerWidget->setMinimumSize(300, (labelnum -1)*70 + 200); // 确保容器部件足够大以启用滚动
    scrollArea->setWidget(containerWidget);
    scrollArea->setWidgetResizable(true); // 使滚动区域的内容可调整大小
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(scrollArea);

    QVBoxLayout *layout = new QVBoxLayout(containerWidget); // 使用垂直布局管理器
    int Xpos = 60;
    int Ypos = 70;
    creatGroup(Xpos,Ypos);
    // 初始化定时器
    hoverTimer = new QTimer(this);
    hoverTimer->setSingleShot(true);
    hoverRelease();

}

SiderListWidegt::~SiderListWidegt()
{
    delete ui;
}

void SiderListWidegt::creatGroup(int Xpos, int Ypos)
{
    int i = 0;
    for (int num = 0; num < labelnum; num ++ ) {
        // 创建 CustomLabel 并设置背景颜色
        CustomLabel *label = new CustomLabel(containerWidget); // 注意这里是 containerWidget
        label->setText(QString("关卡%1").arg(num));
        label->setStyleSheet(QString("background-color: %1;").arg(colors[num%5]));
        label->setFixedSize(300, 100); // 设置 QLabel 大小
        itemlist.append(label);
        //滑动结束动画
        connect(scrollArea,&CustomScrollArea::animationStart,this,&SiderListWidegt::afterScroll);
        //label点击信号
        connect(label,&CustomLabel::labelClicked,this,&SiderListWidegt::selectLabel);
        //状态设置
        connect(scrollArea,&CustomScrollArea::isScrolling,[=](bool scrolling){
            QTimer::singleShot(100,[=](){
                label->setScoll(scrolling);});
        });
        //滑动过程动画
        connect(scrollArea,&CustomScrollArea::sliderMove,[=]() {
            //滑动偏移
            posGeometry(QPoint(label->pos().x() + 10,label->pos().y()),label);
        });
        //label hover动画
        connect(label, &CustomLabel::hoverSignal, this, &SiderListWidegt::hoverAnimation);

    }

    //设置初始位置
    int peek = 2;
    for(auto item : itemlist) {
        if(i == peek){
            item->move(QPoint(Xpos - 10,(i) * Ypos));
        } else {
            int distance = abs(i - peek);
            int x = Xpos + distance * 10; //线性递减
            item->move(QPoint(x, (i) * Ypos));
        }
        i++;

        item->LabelStates[0] = item->pos();
    }
}

//位移动画
void SiderListWidegt::posGeometry(QPoint point, CustomLabel *lb)
{
    QPropertyAnimation *animation = new QPropertyAnimation(lb,"pos");
    animation->setDuration(300);
    animation->setStartValue(lb->pos());
    animation->setEndValue(point);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void SiderListWidegt::updatePos(CustomLabel* item)
{

}

void SiderListWidegt::selectLabel(CustomLabel* lb)
{
    scrollArea->scrolling = true;
    hovering = true;
    int dis;

    for (int index = 0; index < itemlist.size(); ++index)
    {
        if(index == itemlist.indexOf(lb)) {
            dis = peakIndex - index;
            scrollArea->offset  +=  dis;
            peakIndex = index;
            scrollArea->setScorllV((index - 2)*70);

        }
    }

    QTimer::singleShot(0,[=](){
    for (int index = 0; index < itemlist.size(); ++index)
    {
        CustomLabel *label = itemlist.at(index);
        int yPos = label->pos().y();
        int maxX = 60;

        if (index == peakIndex) {
            // 设置峰值位置
            posGeometry(QPoint(maxX - 20, yPos),label);
            int y = label->LabelStates[0].y();
            label->LabelStates[0] = QPoint(maxX - 20, y);
        } else {
            int distance = abs(index - peakIndex);
            int x = maxX + distance * 10;
            int y = label->LabelStates[0].y();
            posGeometry(QPoint(x, yPos),label);
            label->LabelStates[0] = QPoint(x, y);
        }

        label->setScoll(false);

    }
        scrollArea->scrolling = false;
    });



}

void SiderListWidegt::afterScroll(int dir)
{

    auto offset = scrollArea->offset;
    peakIndex = 2 - offset; // 峰值的索引
    if(peakIndex < 2 || peakIndex > 18) {
        return;
    }

    //当前显示item的动画
    for (int index = 0; index < itemlist.size(); ++index)
    {
        CustomLabel *label = itemlist.at(index);
        // int currentIndex = index + offset;//偏移后
        int maxX = 60;
        auto y = label->LabelStates[0].y();
        if (index == peakIndex) {
            // 设置峰值位置
            label->LabelStates[0] = QPoint(maxX - 10, y);
            posGeometry(label->LabelStates[0],label);
        } else {
            int distance = abs(index - peakIndex);
            int x = maxX + distance * 10;
            label->LabelStates[0] = QPoint(x, y);
            posGeometry(label->LabelStates[0],label);
        }
    }

}

void SiderListWidegt::hoverAnimation(bool ishover,CustomLabel* label)
{
    auto cur = itemlist.indexOf(label);//当前label
    // 偏移量
    int _y = 35;
    QPoint endPoint{};
    QPoint hoverPoint{};

    if (hovering == false){
        hoverIndex = cur;//初始
    }

    //进入label信号
    if (ishover == true) {
        hoverTimer->stop();  // 停止定时器，防止误触发
        if (hovering == false) {  // 没有其他标签悬浮
            for (int index = 0; index < itemlist.size(); ++index) {
                CustomLabel *label = itemlist.at(index);
                int x = label->pos().x();
                int y = label->pos().y();

                label->LabelStates[0] = label->pos();//初始值
                label->LabelStates[3] = {x, y - _y};//收起 up
                label->LabelStates[4] = {x, y + _y};//收起 down

                if (index < hoverIndex) {
                    endPoint = {x, y - _y};
                    hoverPoint = {x - 20,y};
                } else if (index > hoverIndex) {
                    endPoint = {x, y + _y};
                    hoverPoint = {x - 20,y};
                } else if (index == hoverIndex) {
                    endPoint = {x - 20, y};
                    hoverPoint = {x - 20,y};
                    previousLabel = label;  // 记录当前悬浮的标签
                }
                label->LabelStates[1] = endPoint;
                label->LabelStates[2] = hoverPoint;
                posGeometry(endPoint, label);  // 左移当前悬浮的标签
            }
            hovering = true;  // 更新为有悬浮状态
        } else {  // 如果已有悬浮的标签，仅移动当前标签
            posGeometry(label->LabelStates[2], label);  // 左移悬浮标签

            int preindex = itemlist.indexOf(previousLabel);
            auto cur = itemlist.indexOf(label);//当前label
            int x = previousLabel->pos().x();
            int y = previousLabel->pos().y();

            if(cur < preindex)
                posGeometry(QPoint(x + 10,y + _y), previousLabel);
            if(cur > preindex)
                posGeometry(QPoint(x + 10,y - _y), previousLabel);

            previousLabel = label;  // 更新当前悬浮的标签
        }
    }
    //离开lable信号
    if (ishover == false) {

        // 启动定时器，500ms后检查是否有新的悬浮事件
        int cur = itemlist.indexOf(label);

        hoverTimer->start(300);
        //定时器结束时收起全部
        QTimer::singleShot(0,[=](){

            // 悬浮状态为 true 时，只收起之前悬浮的标签
            if (hovering == true) {

                //posGeometry(label->LabelStates[5], label);
            }
        });
    }

}

//全部收起
void SiderListWidegt::hoverRelease()
{
    connect(hoverTimer, &QTimer::timeout, this, [=]() {
        hovering = false;  // 500ms后无新的悬浮事件，将悬浮状态重置
        for (int index = 0; index < itemlist.size(); ++index) {
            CustomLabel *label = itemlist.at(index);
            if(index == hoverIndex){
                posGeometry(label->LabelStates[0], label);
            } else if (index < hoverIndex) {
                posGeometry(label->LabelStates[0], label);
            } else if (index > hoverIndex) {
                posGeometry(label->LabelStates[0], label);
            }
        }
    });
}


