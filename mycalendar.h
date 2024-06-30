#ifndef MYCALENDAR_H
#define MYCALENDAR_H

#include <QWidget>
#include <QCalendarWidget>
#include <QDate>
#include <QMap>
#include <QFont>
#include <QVBoxLayout>
#include <QTextCharFormat>
#include <QLocale>
#include <QFile>
#include <QTextStream>
namespace Ui {
class mycalendar;
}

class mycalendar : public QWidget
{
    Q_OBJECT

public:
    explicit mycalendar(QWidget *parent = nullptr);
    ~mycalendar();

private:
    Ui::mycalendar *ui;
    QCalendarWidget *calendar;
    QMap<QDate, QString> diaries;  // 用于存储日期和日记内容的映射
    void updateCalendar();
    void saveDiaries();
    void loadDiaries();

private slots:
    void onDateClicked(const QDate &date);

protected:
    //void paintEvent(QPaintEvent *event) override;
    //void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const;

};

#endif // MYCALENDAR_H
