#include "jottings.h"
#include "ui_jottings.h"
#include "global.h"
#include <QContextMenuEvent>
#include <QAction>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QComboBox>
#include <QTimer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <algorithm>
#include <QDate>
#include <QMessageBox>
#include <QTextStream>

Jottings::Jottings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Jottings)
    , currentTextBox(nullptr)
{
    ui->setupUi(this);
    currentDate = QDate::currentDate().toString("yyyyMMdd");
    filePath = globalFilePath + '/'+ currentDate + " Jotting.txt";
    // 创建主垂直布局
    mainLayout = new QVBoxLayout(this);

    // 创建文本框垂直布局
    layout = new QVBoxLayout();

    // 初始化添加按钮
    addButton = new QToolButton(this);
    addButton->setIcon(QIcon(":/new/prefix1/res/pic/plus.png"));
    addButton->setFixedSize(60, 60);

    // 初始化保存按钮
    QPushButton *saveButton = new QPushButton(this);
    saveButton->setIcon(QIcon(":/images/res/pic/save.png"));
    saveButton->setFixedSize(60, 60);  // 设置保存按钮的固定大小

    // 创建一个水平布局来包含添加按钮和保存按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(); // 添加一个弹性空间以将按钮推到右边
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(addButton);

    // 将文本框布局和弹性空间添加到主垂直布局
    mainLayout->addLayout(layout);
    mainLayout->addStretch(); // 添加一个弹性空间以将按钮推到最下方
    mainLayout->addLayout(buttonLayout);

    // 连接按钮点击信号到槽函数
    connect(addButton, &QToolButton::clicked, this, &Jottings::onAddButtonClicked);
    connect(saveButton, &QPushButton::clicked, this, &Jottings::onSaveButtonClicked);

    // 初始化右键菜单
    contextMenu = new QMenu(this);
    QAction *deleteAction = new QAction("删除", this);
    contextMenu->addAction(deleteAction);
    connect(deleteAction, &QAction::triggered, this, &Jottings::onDeleteTextBox);

    // 设置主布局
    setLayout(mainLayout);

    // 加载文本框
    loadTextBoxes();
}


Jottings::~Jottings()
{
    delete ui;
}

void Jottings::closeEvent(QCloseEvent *event)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "错误", "无法打开文件进行保存");
        return;
    }

    QTextStream out(&file);
    for (int i = 0; i < textBoxes.size(); ++i)
    {
        QLineEdit *textBox = textBoxes[i];
        QComboBox *comboBox = comboBoxes[i]; // 获取对应的下拉菜单
        out << textBox->text() << "|" << comboBox->currentIndex() << "\n";
    }

    file.close();
    event->accept();
}

void Jottings::onAddButtonClicked()
{
    createTextBox();
}

void Jottings::createTextBox(const QString &text, int comboIndex)
{
    // 创建文本框
    QLineEdit *textBox = new QLineEdit(this);
    textBox->setText(text);
    textBoxes.append(textBox);

    // 创建小白框
    QLabel *iconBox = new QLabel(this);
    iconBox->setFixedSize(30, 30);
    iconBox->setStyleSheet("background-color: white; border: 1px solid black;");

    // 创建下拉菜单
    QComboBox *comboBox = new QComboBox(this);
    createComboBox(comboBox, iconBox);
    comboBox->setCurrentIndex(comboIndex);
    comboBoxes.append(comboBox);

    // 创建水平布局
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(iconBox);
    hLayout->addWidget(comboBox);
    hLayout->addWidget(textBox);

    // 在水平布局和下一个文本框之间添加固定间距
    layout->addLayout(hLayout);
    layout->addSpacing(10);  // 添加一个10像素的固定间距

    // 设置文本框的上下文菜单策略为自定义
    textBox->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(textBox, &QLineEdit::customContextMenuRequested, [this, textBox](const QPoint &pos) {
        currentTextBox = textBox;
        contextMenu->exec(textBox->mapToGlobal(pos));
    });

    // 连接文本框编辑完成信号到槽函数
    connect(textBox, &QLineEdit::editingFinished, this, &Jottings::handleEditingFinished);

    // 使文本框获取焦点
    textBox->setFocus();
}

void Jottings::createComboBox(QComboBox *comboBox, QLabel *iconBox)
{
    // 添加下拉菜单项并连接信号槽
    QPixmap pixmap1(":/new/prefix1/res/pic/rain.jpeg");
    QPixmap pixmap2(":/new/prefix1/res/pic/call.jpeg");
    QPixmap pixmap3(":/new/prefix1/res/pic/shop.jpeg");
    QPixmap pixmap4(":/new/prefix1/res/pic/study.jpeg");
    QPixmap pixmap5(":/new/prefix1/res/pic/exercise.png");
    QPixmap pixmap6(":/new/prefix1/res/pic/music.jpeg");
    comboBox->addItem("下雨", pixmap1);
    comboBox->addItem("电话", pixmap2);
    comboBox->addItem("购物", pixmap3);
    comboBox->addItem("学习", pixmap4);
    comboBox->addItem("运动", pixmap5);
    comboBox->addItem("音乐", pixmap6);

    connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [iconBox, comboBox](int index) {
        QVariant data = comboBox->itemData(index);
        if (data.canConvert<QPixmap>()) {
            QPixmap pixmap = data.value<QPixmap>();
            if (!pixmap.isNull()) {
                iconBox->setPixmap(pixmap.scaled(30, 30, Qt::KeepAspectRatio));
            }
        }
    });

    // 设置默认选择项为“学习”并更新图标
    int defaultIndex = 3; // 学习的索引是3
    comboBox->setCurrentIndex(defaultIndex);
    QVariant data = comboBox->itemData(defaultIndex);
    if (data.canConvert<QPixmap>()) {
        QPixmap initialPixmap = data.value<QPixmap>();
        iconBox->setPixmap(initialPixmap.scaled(30, 30, Qt::KeepAspectRatio));
    }
}


void Jottings::handleEditingFinished()
{
    QLineEdit *textBox = qobject_cast<QLineEdit*>(sender());
    if (textBox && textBox->text().isEmpty())
    {
        //textBoxes.removeOne(textBox);
        //layout->removeWidget(textBox);
        //delete textBox;
    }
}

void Jottings::contextMenuEvent(QContextMenuEvent *event)
{
    currentTextBox = qobject_cast<QLineEdit*>(childAt(event->pos()));
    if (currentTextBox)
    {
        contextMenu->exec(event->globalPos());
    }
}

void Jottings::onDeleteTextBox()
{
    if (currentTextBox)
    {
        QLayoutItem *itemToRemove = nullptr;
        QLayoutItem *spacerItemToRemove = nullptr;

        // 查找包含当前文本框的水平布局
        for (int i = 0; i < layout->count(); ++i)
        {
            QLayoutItem *item = layout->itemAt(i);
            QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(item->layout());
            if (hLayout && hLayout->indexOf(currentTextBox) != -1)
            {
                // 移除并删除布局中的所有小部件
                QLayoutItem *child;
                while ((child = hLayout->takeAt(0)) != nullptr)
                {
                    QWidget *widget = child->widget();
                    if (widget)
                    {
                        widget->deleteLater();
                    }
                    delete child;
                }

                // 准备删除的项
                itemToRemove = item;

                // 查找与当前水平布局相邻的固定间距项
                if (i < layout->count() - 1)
                {
                    spacerItemToRemove = layout->itemAt(i + 1);
                }

                break;
            }
        }

        // 删除当前水平布局及其相邻的固定间距项
        if (itemToRemove)
        {
            layout->removeItem(itemToRemove);
            delete itemToRemove;

            if (spacerItemToRemove)
            {
                layout->removeItem(spacerItemToRemove);
                delete spacerItemToRemove;
            }
        }

        // 从数据结构中移除当前文本框
        int index = textBoxes.indexOf(currentTextBox);
        if (index != -1)
        {
            textBoxes.removeAt(index);
            comboBoxes.removeAt(index);
        }

        // 删除当前文本框对象
        currentTextBox->deleteLater();
        currentTextBox = nullptr;

        // 更新布局
        layout->update();
    }
}

void Jottings::onSaveButtonClicked()
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "错误", "无法打开文件进行保存");
        return;
    }

    QTextStream out(&file);
    for (int i = 0; i < textBoxes.size(); ++i)
    {
        QLineEdit *textBox = textBoxes[i];
        QComboBox *comboBox = comboBoxes[i]; // 获取对应的下拉菜单
        out << textBox->text() << "|" << comboBox->currentIndex() << "\n";
    }

    file.close();
    QMessageBox::information(this, "保存成功", "文本已成功保存");
}

void Jottings::loadTextBoxes()
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "无法打开文件进行读取";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split("|");
        if (parts.size() == 2)
        {
            QString text = parts[0];
            int iconIndex = parts[1].toInt();
            createTextBox(text, iconIndex); // 使用保存的图标索引创建文本框
        }
    }

    file.close();
}
