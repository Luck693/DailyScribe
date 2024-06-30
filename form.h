#ifndef FORM_H
#define FORM_H
#include "inkwidget.h"  // 包含InkWidget的头文件
#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
#include <QStringList>
#include "diaries.h"
#include "jottings.h"
#include "recollections.h"
#include "settings.h"
QT_BEGIN_NAMESPACE
namespace Ui {class Form;}
QT_END_NAMESPACE

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

private:
    Ui::Form *ui;
    InkWidget *inkWidget;  // 墨迹绘制部件
    Diaries* diary;
    Jottings* jotting;
    Recollections* recollection;
    Settings* setting;
    QTimer *quoteTimer;
    QStringList quotes;
    void loadQuotes();
private slots:
    void updateQuote();
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
};

#endif // FORM_H
