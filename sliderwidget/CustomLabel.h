#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QCursor>
#include <QDebug>
#include <QEvent>
#include <QPropertyAnimation>
#include <QMap>
// 自定义 CustomLabel 类，继承 QLabel
class CustomLabel : public QLabel {
    Q_OBJECT

public:
    explicit CustomLabel(QWidget *parent = nullptr);

    QMap<int,QPoint> LabelStates;//label状态

    bool isScoll = false;//scrollarea是否为滚动状态
    static bool pending;

    void setScoll(bool p) {
        isScoll = p;
    }

    int AmtTime = 500;


protected:
    // 鼠标进入事件
    void enterEvent(QEnterEvent *event) override;

    // 鼠标离开事件
    void leaveEvent(QEvent *event) override;

    // 鼠标点击事件
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void labelClicked(CustomLabel* lb); // 自定义信号，用于鼠标点击时的处理
    void hoverSignal(bool ishover,CustomLabel* label); // 悬浮信号，传递当前label和状态

};
