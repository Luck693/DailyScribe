#ifndef REGIS_H
#define REGIS_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>

namespace Ui {
class regis;
}

class regis : public QWidget
{
    Q_OBJECT

public:
    QSqlDatabase db;
    explicit regis(QWidget *parent = nullptr);
    ~regis();

private slots:
    void on_button_register_clicked();

    void on_button_back_clicked();

private:
    Ui::regis *ui;
};

#endif // REGIS_H
