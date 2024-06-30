#include "recollections.h"
#include "ui_recollections.h"
#include "diarydialog.h"
#include "global.h"
#include <QDebug>
#include <QMessageBox>
Recollections::Recollections(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Recollections)
{
    ui->setupUi(this);
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 创建 QCalendarWidget 实例并添加到布局中
    calendar = new QCalendarWidget(this);
    calendar->setFirstDayOfWeek(Qt::Monday);
    calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    calendar->setFont(QFont("Times New Roman", 12,QFont::Bold));


    // 设置样式表
    calendar->setStyleSheet(
        "QCalendarWidget QAbstractItemView:enabled {"
        "    background: #FFFFFF;"  // 背景颜色
        "    color: #4682B4;"              // 文本颜色
        "    selection-background: #00A572;"  // 选中日期的背景颜色
        "    selection-color: #00bfff;"             // 选中日期的文本颜色
        "    border: 1px solid #00A572;"            // 边框颜色
        "    border-radius: 10px;"                  // 边框圆角
        "}"
        "QCalendarWidget QToolButton {"
        "    background: #ffffff;"  // 月份选择按钮的背景颜色
        "    color: #000000;"             // 月份选择按钮的文本颜色
        "    border: none;"               // 按钮边框
        "    border-radius: 5px;"         // 按钮圆角
        "    padding: 5px;"
        "}"
        "QCalendarWidget QAbstractItemView:enabled::item:selected {"
        "    border-radius: 10px;"  // 圆形边界
        "    margin: 2px;"          // 控制间距
        "}"
        "QCalendarWidget QToolButton:hover {"
        "    color: #E34234;"
        "}"
        "QCalendarWidget QWidget#qt_calendar_navigationbar {"
        "    background: #FFFFFF;"  // 导航栏背景颜色
        "}"
        "QCalendarWidget QSpinBox {"
        "    margin: 5px;"  // 微调框的边距
        "}"
        "QCalendarWidget QSpinBox::up-button {"
        "    subcontrol-origin: border;"
        "    subcontrol-position: top right;"
        "    width: 15px;"
        "}"
        "QCalendarWidget QSpinBox::down-button {"
        "    subcontrol-origin: border;"
        "    subcontrol-position: bottom right;"
        "    width: 15px;"
        "}"
        "QCalendarWidget QComboBox {"
        "    background: #FFFFFF;"  // 下拉列表的背景颜色
        "    color: #000000;"  // 下拉列表的文字颜色
        "    border: 1px solid #d4d4d4;"  // 下拉列表的边框
        "}"
        "QCalendarWidget QComboBox QAbstractItemView {"
        "    background-color: #E0E0E0;"  // 项目视图的背景颜色
        "    color: #000000;"  // 项目视图的文字颜色
        "}"
        "QCalendarWidget QToolButton#qt_calendar_prevmonth {"
        "    qproperty-icon: url(:/images/res/pic/left_arrow.png);"  /* 左箭头图标 */
        "width: 25px;"  /* 按钮宽度 */
        "height: 25px;"  /* 按钮高度 */
        "}"
        "QCalendarWidget QToolButton#qt_calendar_nextmonth {"
        "    qproperty-icon: url(:/images/res/pic/right_arrow.png); " /* 右箭头图标 */
        "width: 25px;"  /* 按钮宽度 */
        "height: 25px;"  /* 按钮高度 */
        "}"
        );
    connect(calendar, &QCalendarWidget::clicked, this, &Recollections::onDateClicked);
    layout->addWidget(calendar);
    calendar->setFixedSize(400, 300);
    layout->setAlignment(calendar, Qt::AlignHCenter);
    setLayout(layout);
    updateCalendar();
}

Recollections::~Recollections()
{
    delete calendar;
    delete ui;
}
void Recollections::onDateClicked(const QDate &date)
{qDebug()<<"kk"<<fileContent;
    // 如果当前日期已有日记，则加载到对话框中
    if (fileContent.contains(date.toString("yyyyMMdd"))) {
        QString filePath = globalFilePath + "/" + date.toString("yyyyMMdd") + ".html";

        DiaryDialog *viewer = new DiaryDialog(filePath);
        viewer->show();
    }

}
void Recollections::updateCalendar()
{
    QTextCharFormat format;
    format.setBackground(Qt::yellow);  // 设置有日记的日期背景为黄色

    // 清除之前的标记
    calendar->setDateTextFormat(QDate(), QTextCharFormat());
    QFile file2(globalFilePath+'/'+"diaries_seq.txt");
    if (!file2.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "hhhhhhhhhhhhhhhhhh无法打开文件";
        return;
    }
    QTextStream in(&file2);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QDate date = QDate::fromString(line, "yyyyMMdd");
        if (date.isValid()) {
            calendar->setDateTextFormat(date, format);
            qDebug()<<"1";
        } else {
            qWarning() << "无效日期格式:" << line;
        }
    }
    file2.seek(0);
    fileContent = in.readAll();
    file2.close();
}
