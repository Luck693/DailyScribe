#include "mycalendar.h"
#include "ui_mycalendar.h"
#include "diarydialog.h"
#include <QDebug>

mycalendar::mycalendar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mycalendar)
{
    ui->setupUi(this);
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 创建 QCalendarWidget 实例并添加到布局中
    calendar = new QCalendarWidget(this);
    calendar->setFirstDayOfWeek(Qt::Monday);
    calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    calendar->setFont(QFont("Times New Roman", 12));


    // 设置样式表
    calendar->setStyleSheet(
        "QCalendarWidget QAbstractItemView:enabled {"
        "    background-color: #D3D3D3;"  // 背景颜色
        "    color: #000000;"              // 文本颜色
        "    selection-background-color: #d3d3d3;"  // 选中日期的背景颜色
        "    selection-color: #00bfff;"             // 选中日期的文本颜色
        "    border: 1px solid #d4d4d4;"            // 边框颜色
        "    border-radius: 10px;"                  // 边框圆角
        "}"
        "QCalendarWidget QToolButton {"
        "    background-color: #d3d3d3;"  // 月份选择按钮的背景颜色
        "    color: #ffffff;"             // 月份选择按钮的文本颜色
        "    border: none;"               // 按钮边框
        "    border-radius: 5px;"         // 按钮圆角
        "    padding: 5px;"
        "}"
        "QCalendarWidget QAbstractItemView:enabled::item:selected {"
        "    border-radius: 10px;"  // 圆形边界
        "    margin: 2px;"          // 控制间距
        "}"
        "QCalendarWidget QToolButton:hover {"
        "    background-color: #fffacd;"  // 按钮悬停背景颜色
        "    color: #000000;"
        "}"
        "QCalendarWidget QWidget#qt_calendar_navigationbar {"
        "    background-color: #e0e0e0;"  // 导航栏背景颜色
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
        "    margin: 5px;"  // 下拉框的边距
        "}"
        "QCalendarWidget QComboBox QAbstractItemView {"
        "    background-color: #ffffff;"  // 下拉列表的背景颜色
        "    border: 1px solid #d4d4d4;"  // 下拉列表的边框
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
    connect(calendar, &QCalendarWidget::clicked, this, &mycalendar::onDateClicked);
    layout->addWidget(calendar);
    calendar->setFixedSize(400, 300);
    layout->setAlignment(calendar, Qt::AlignHCenter);

    setLayout(layout);

    loadDiaries();
    updateCalendar();
}
mycalendar::~mycalendar()
{
    saveDiaries();
    delete ui;
}
void mycalendar::onDateClicked(const QDate &date)
{
    diarydialog dialog(this);

    // 如果当前日期已有日记，则加载到对话框中
    if (diaries.contains(date)) {
        dialog.setDiaryText(diaries[date]);
    }

    if (dialog.exec() == QDialog::Accepted) {
        QString diaryText = dialog.getDiaryText();
        if (!diaryText.isEmpty()) {
            diaries[date] = diaryText;
        } else {
            diaries.remove(date);  // 如果文本为空，则移除该日记
        }
        updateCalendar();
    }
}
void mycalendar::updateCalendar()
{
    QTextCharFormat format;
    format.setBackground(Qt::yellow);  // 设置有日记的日期背景为黄色

    // 清除之前的标记
    calendar->setDateTextFormat(QDate(), QTextCharFormat());
    for (auto it = diaries.begin(); it != diaries.end(); ++it) {
        calendar->setDateTextFormat(it.key(), format);
    }
}
void mycalendar::saveDiaries()
{
    QFile file("diaries.txt");
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        diaries[QDate::currentDate()]="1";
        for (auto it = diaries.begin(); it != diaries.end(); ++it) {
            out << it.key().toString(Qt::ISODate) << "\n";
            out << it.value() << "\n";
        }
    }
}
void mycalendar::loadDiaries()
{
    QFile file("diaries.txt");
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString dateStr = in.readLine();
            QString diaryText = in.readLine();
            QDate date = QDate::fromString(dateStr, Qt::ISODate);
            if (date.isValid()) {
                diaries[date] = diaryText;
            }
        }
    }
}
