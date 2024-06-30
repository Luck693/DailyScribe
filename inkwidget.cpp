#include <QMouseEvent>
#include <QPainter>
#include <QEvent>
#include <QObject>
#include <QTimer>
#include <qdebug.h>
#include "InkWidget.h"
#include "ui_inkwidget.h"
InkWidget::InkWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::InkWidget)
    ,updateTimer(new QTimer(this))
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StaticContents);
    setMouseTracking(true);
    connect(updateTimer, &QTimer::timeout, this, &InkWidget::updateInkPoints);
    updateTimer->start(20); // 每20毫秒更新一次
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);

}

InkWidget::~InkWidget() {
    delete ui;
}

void InkWidget::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}
void InkWidget::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void InkWidget::mouseMoveEvent(QMouseEvent *event) {//加入所有要画的点
    if (!inkPoints.isEmpty()) {
        QPointF lastPoint = inkPoints.last();
        interpolatePoints(lastPoint, event->pos()+QPoint(20,20));//插点
    } else {
        addInkPoint(event->pos()+QPoint(20,20));
    }
    update();
}

void InkWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    for (int i = 0; i < inkPoints.size(); ++i) {
        painter.setBrush(inkColors[i]);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(inkPoints[i], 2, 2);  // 绘制半径2的圆
    }
}

void InkWidget::updateInkPoints() {
    bool updated = false;

    for (int i = 0; i < inkPoints.size(); ++i) {
        QColor color = inkColors[i];
        int alpha = color.alpha() - 20;  // 每次减少透明度
        if (alpha < 0) alpha = 0;
        color.setAlpha(alpha);
        inkColors[i] = color;

        if (alpha == 0) {
            inkPoints.remove(i);
            inkColors.remove(i);
            --i;  // 调整索引以匹配已删除的项
        }
        updated = true;
    }

    if (updated) {
        update(); // 触发重绘
    }
}
void InkWidget::addInkPoint(const QPointF &point) {
    inkPoints.append(point);
    inkColors.append(QColor(0, 0, 0, 255));  // 初始颜色为不透明黑色
}

void InkWidget::interpolatePoints(const QPointF &start, const QPointF &end) {
    const qreal distance = std::sqrt(std::pow(end.x() - start.x(), 2) + std::pow(end.y() - start.y(), 2));
    const int steps = static_cast<int>(distance / 2.0); // 调整采样率，这里为每2个像素采样一次

    for (int i = 1; i <= steps; ++i) {
        qreal t = static_cast<qreal>(i) / steps;
        QPointF interpolatedPoint = start + t * (end - start);
        addInkPoint(interpolatedPoint);
    }
}
