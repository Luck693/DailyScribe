#ifndef RECOLLECTIONS_H
#define RECOLLECTIONS_H

#include <QWidget>
#include <QCalendarWidget>
#include <QDate>
#include <QVector>
#include <QFont>
#include <QVBoxLayout>
#include <QTextCharFormat>
#include <QLocale>
#include <QFile>
#include <QTextStream>
#include <QString>
namespace Ui {
class Recollections;
}

class Recollections : public QWidget
{
    Q_OBJECT

public:
    explicit Recollections(QWidget *parent = nullptr);
    ~Recollections();

private slots:
    void onDateClicked(const QDate &date);

private:
    Ui::Recollections *ui;
    QCalendarWidget *calendar;
    QString fileContent;
    void updateCalendar();

};

#endif // RECOLLECTIONS_H
