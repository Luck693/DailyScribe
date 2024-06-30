#include "regis.h"
#include "ui_regis.h"
#include "page_login.h"
#include "form.h"
#include "global.h"
#include <QFileDialog>
#include <QFile>
#include <QString>
#include <QMessageBox>

regis::regis(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::regis)
{
    ui->setupUi(this);
    this -> setWindowTitle("用户注册");
    ui->let_passwd1->setEchoMode(QLineEdit::Password);
    ui->let_passwd2->setEchoMode(QLineEdit::Password);
    ui->let_passwd1->setMaxLength(8);
    ui->let_passwd2->setMaxLength(8);
}

regis::~regis()
{
    delete ui;
}
//跳转到登入界面
void regis::on_button_register_clicked()
{
    QString str1 = ui->let_passwd1->text();
    QString str2 = ui->let_passwd2->text();

    // 检查密码长度是否符合要求
    if (str1.length() < 8 || str2.length() < 8)
    {
        QMessageBox::about(this, "错误", "密码长度不符合要求");
        return;
    }

    // 检查两次输入的密码是否相同
    if (str1 != str2)
    {
        QMessageBox::about(this, "错误", "两次密码输入不同");
        return;
    }

    // 连接数据库
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("user.db");
    if (!db.open())
    {
        QMessageBox::critical(this, "错误", "无法连接到数据库");
        return;
    }

    // 检查用户名是否已存在
    QSqlQuery query(db);
    query.prepare("SELECT username FROM userInfo WHERE username = :username");
    query.bindValue(":username", ui->let_user->text());
    if (!query.exec())
    {
        QMessageBox::critical(this, "错误", "查询用户名失败");
        return;
    }
    if (query.next())
    {
        QMessageBox::about(this, "提示", "用户名已存在");
        return;
    }

    // 执行插入操作
    query.prepare("INSERT INTO userInfo (username, password) VALUES (:username, :password)");
    query.bindValue(":username", ui->let_user->text());
    query.bindValue(":password", str2); // 使用第二次输入的密码
    if (!query.exec())
    {
        QMessageBox::critical(this, "错误", "注册失败");
        return;
    }
    QMessageBox::about(this, "提示", "您还需要选择本地文件夹用来存储您的日记");
    globalFilePath = QFileDialog::getExistingDirectory(nullptr,
                                                           QObject::tr("选择文件夹"),
                                                           "",
                                                           QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QFile file("user_location.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        qWarning() << "1";
        return;
    }
    QTextStream out(&file);
    out << ui->let_user->text() << '\n'<< globalFilePath<<'\n';
    file.close();

    QMessageBox::about(this, "提示", "新用户注册成功");
    this->hide();
    Page_Login *w = new Page_Login();
    w->show();
}

//跳转到登入界面
void regis::on_button_back_clicked()
{
    Page_Login *w = new Page_Login();
    w -> show();
    this -> hide();
}

