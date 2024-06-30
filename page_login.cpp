#include "page_login.h"
#include "ui_page_login.h"
#include "form.h" // include the Form header file
#include "regis.h"
#include "global.h"
#include <QMessageBox>
#include <QDebug>

Page_Login::Page_Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page_Login)
{
    ui->setupUi(this);
    this->setWindowTitle("用户登入");
    ui->let_passwd->setEchoMode(QLineEdit::Password);
    ui->let_passwd->setMaxLength(8);

    // 创建或连接数据库
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("user.db");
    if (!db.open()) {
        QMessageBox::critical(this, "错误", "无法连接到数据库");
        return;
    }

    // 检查表是否存在，若不存在则创建
    QSqlQuery query;
    if (!query.exec("SELECT * FROM sqlite_master WHERE type='table' AND name='userInfo'")) {
        QMessageBox::critical(this, "错误", "无法检查数据库表是否存在");
        return;
    }
    if (!query.next()) {
        if (!query.exec("CREATE TABLE userInfo (username VARCHAR(20), password VARCHAR(20))")) {
            QMessageBox::critical(this, "错误", "无法创建表格");
            return;
        }
    }
}

Page_Login::~Page_Login()
{
    delete ui;
}

void Page_Login::on_button_login_clicked()
{
    QString username = ui->let_user->text().trimmed(); // 去除首尾空格
    QString password = ui->let_passwd->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::critical(this, "错误", "请输入用户名和密码");
        return;
    }

    // 查询数据库中是否存在匹配的用户名和密码
    QSqlQuery query(db);
    query.prepare("SELECT * FROM userInfo WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "查询失败");
        return;
    }

    if (query.next()) { // 如果存在匹配项，登录成功
        this->hide();
        QFile file("user_location.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text )) {
            qWarning() << "1";
            return;
        }
        Username = username;
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line == username) {
                if (!in.atEnd()) {
                    QString FilePath = in.readLine();
                    file.close();
                    globalFilePath = FilePath;
                }
            }
        }
        Form *form = new Form(); // Create Form instance
        form->show(); // Show the Form
    } else {
        QMessageBox::critical(this, "错误", "账号或密码错误");
    }
}

void Page_Login::on_button_sign_clicked()
{
    regis *w = new regis();
    w->show();
    this->hide();
}

void Page_Login::on_pushButton_clicked(bool checked)
{
    // 切换密码显示模式
    ui->let_passwd->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}

void Page_Login::on_checkBox_stateChanged(int arg1)
{
    // 切换密码显示模式
    ui->let_passwd->setEchoMode(arg1 ? QLineEdit::Normal : QLineEdit::Password);
}
