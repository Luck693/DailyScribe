#ifndef INKWIDGET_H
#define INKWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPointF>
#include <QColor>
#include <QTimer>

namespace Ui {
class InkWidget;
}
class InkWidget : public QWidget {
    Q_OBJECT

public:
    explicit InkWidget(QWidget *parent = nullptr);
    ~InkWidget();
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private slots:
    void updateInkPoints(); // 用于更新点透明度的方法

private:
    QVector<QPointF> inkPoints;
    QVector<QColor> inkColors;
    Ui::InkWidget *ui;
    QTimer *updateTimer; // 定时器
    void addInkPoint(const QPointF &point);//编辑inkPoints和inkColors
    void interpolatePoints(const QPointF &start, const QPointF &end);//增加点使轨迹更加流畅
};

#endif // INKWIDGET_H
