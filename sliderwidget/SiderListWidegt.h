#ifndef SIDERLISTWIDEGT_H
#define SIDERLISTWIDEGT_H

#include <QWidget>
#include <QScrollArea>
#include <QLayout>

QT_BEGIN_NAMESPACE
class CustomLabel;
class CustomScrollArea;

namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class SiderListWidegt : public QWidget
{
    Q_OBJECT

public:
    SiderListWidegt(QWidget *parent = nullptr);
    ~SiderListWidegt();

    int peakIndex = 2;
    int maxX = 60;
    bool hovering = false;//当前界面是否有hover状态的label
    int hoverIndex;

private:
    Ui::Widget *ui;
    CustomScrollArea *scrollArea;
    QWidget *containerWidget; // 容器部件
    QVBoxLayout *layout;      // 布局管理器
    QTimer *hoverTimer;            // 控制500ms延迟的定时器
    CustomLabel *previousLabel;    // 追踪上一个悬浮的label
    int labelnum = 20;

    void creatGroup(int Xpos, int Ypos);

    void posGeometry(QPoint point,CustomLabel* lb);

    void updatePos(CustomLabel* item);
    void selectLabel(CustomLabel* lb);
    void afterScroll(int dir);
    void hoverAnimation(bool ishover,CustomLabel* label);
    void hoverRelease();
signals:

private slots:


};
#endif // SIDERLISTWIDEGT_H
