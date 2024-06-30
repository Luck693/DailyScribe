#ifndef PAGE_LOGIN_H
#define PAGE_LOGIN_H
#include <QWidget>
#include <QPainter>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>
namespace Ui {
class Page_Login;
}

class Page_Login : public QWidget
{
    Q_OBJECT

public:
    QSqlDatabase db;
    explicit Page_Login(QWidget *parent = nullptr);
    ~Page_Login();
private slots:
    void on_button_login_clicked();
    void on_button_sign_clicked();
    void on_pushButton_clicked(bool checked);
    void on_checkBox_stateChanged(int arg1);

private:
    Ui::Page_Login *ui;
};

#endif // PAGE_LOGIN_H
