#include <QScrollArea>
#include <QWheelEvent>
#include <QScrollBar>
#include <QPropertyAnimation>
#include <QTimer>
#include <QQueue>
class CustomScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    CustomScrollArea(QWidget *parent = nullptr);
    QTimer *timer;//滚轮定时器
    int previousValue = 0;
    int firstValue = 0;
    bool scrolling = false;
    int offset = 0;//peak index偏移量

    QPropertyAnimation *animation;
    QScrollBar *scrollBar;

    void setScorllV(int end);

protected:
    void wheelEvent(QWheelEvent *event) override;


private slots:
    void startScrollBarAnimation();
    void onScrollTimeout();


signals:
    void animationStart(int dir);
    void sliderMove(int dir);
    void isScrolling(bool scrolling);
};
