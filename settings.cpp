#include "settings.h"
#include "ui_settings.h"
#include "global.h"
Settings::Settings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);
    ui->label_username->setText("用户名："+Username);
    ui->label_location->setText("存储地址"+globalFilePath);
    ui->label_info->setText("开发者：张致铨，杨炅凡，羊瑞\n2024.6.1\n欢迎大家使用！");
}

Settings::~Settings()
{
    delete ui;
}
