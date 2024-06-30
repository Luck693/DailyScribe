#include "diaries.h"
#include "ui_diaries.h"
#include "global.h"
#include <QPainter>
#include <QFontComboBox>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDate>
#include <QCloseEvent>
#include <QTextImageFormat>
#include <QFileDialog>
#include <QBuffer>
Diaries::Diaries(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Diaries)
{
    ui->setupUi(this);
    ui->toolButtonColor->setIcon(generateIconWithColor(Qt::black));
    ui->toolButtonColor->installEventFilter(this);
    connect(ui->fontComboBox, SIGNAL(currentIndexChanged(QString)),ui->textEdit, SLOT(setFontFamily(QString)));
    ui->lineEditFontSize->setText( QString("").setNum(12) );
    QFont defaultFont = ui->textEdit->font();
    defaultFont.setPointSize(12); // 设置默认字号为12
    ui->textEdit->setFont(defaultFont);
    currentDate = QDate::currentDate().toString("yyyyMMdd");
    filePath = globalFilePath + '/'+ currentDate + ".html";
    qDebug()<<filePath<<"\n";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "警告", "无法创建文件: " + file.errorString());
            return;
        }
        QTextStream out(&file);
        out << ui->textEdit->toHtml();
        file.close();
    } else {
        // 如果文件存在，则读取内容
        QTextStream in(&file);
        QString content = in.readAll();
        ui->textEdit->setHtml(content);
        file.close();
    }
    ui->textEdit->document()->setModified(false);
}

Diaries::~Diaries()
{
    delete ui;
}


void Diaries::on_toolButtonBold_clicked()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if (cursor.hasSelection()) {
        QTextCharFormat format;
        format.setFontWeight(ui->toolButtonBold->isChecked() ? QFont::Bold : QFont::Normal);
        cursor.mergeCharFormat(format);
        ui->textEdit->mergeCurrentCharFormat(format);
    }
}

void Diaries::closeEvent(QCloseEvent *event) {
    if (ui->textEdit->document()->isModified()){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "保存", "您想要保存更改吗？",
                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (reply == QMessageBox::Yes) {
        on_toolButtonSave_clicked();
        event->accept();
    } else if (reply == QMessageBox::No) {
        event->accept();
    } else {
        event->ignore();
    }
}
}
void Diaries::on_textEdit_cursorPositionChanged()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if (cursor.charFormat().fontWeight() == QFont::Bold) {
        ui->toolButtonBold->setChecked(true);
    } else {
        ui->toolButtonBold->setChecked(false);
    }
    QTextCharFormat format = cursor.charFormat();
    ui->toolButtonItalic->setChecked(format.fontItalic());
    ui->toolButtonUnderline->setChecked(format.fontUnderline());
    qreal fontSize = format.fontPointSize();
    if (fontSize != 0) {
        ui->lineEditFontSize->setText(QString::number(fontSize));
    } else {
        // 如果当前字体大小未设置，使用默认值
        QFont defaultFont = ui->textEdit->font();
        ui->lineEditFontSize->setText(QString::number(defaultFont.pointSize()));
    }
    QFont curFont = format.font();      //获取 QFont 成员
    int nFontSize = curFont.pointSize();//字号检测
    //如果是 px 形式， QFont::​pointSize() 函数返回 -1
    if( -1 == nFontSize ) //如果 pt 是 -1，是 px 格式
    {
        //如果是 px ，换算成 pt
        nFontSize = (int)( curFont.pixelSize() * 9.0 / 12.0 ) ;
    }
    ui->lineEditFontSize->setText( QString("").setNum(nFontSize) );
}

void Diaries::on_toolButtonItalic_clicked()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if (cursor.hasSelection()) {
        QTextCharFormat format;
        format.setFontItalic(ui->toolButtonItalic->isChecked());
        cursor.mergeCharFormat(format);
        ui->textEdit->mergeCurrentCharFormat(format);
    }
}

void Diaries::on_toolButtonUnderline_clicked()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if (cursor.hasSelection()) {
        QTextCharFormat format;
        format.setFontUnderline(ui->toolButtonUnderline->isChecked());
        cursor.mergeCharFormat(format);
        ui->textEdit->mergeCurrentCharFormat(format);
    }
}

bool Diaries::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->toolButtonColor)
    {
        if (event->type() == QEvent::MouseButtonDblClick)
        {
            // 处理双击事件
            QColor clr = QColorDialog::getColor(Qt::black); // 默认是黑色文字
            if (clr.isValid()) // 如果用户选了颜色
            {
                ui->textEdit->setTextColor(clr);
                // 更新按钮的图标
                ui->toolButtonColor->setIcon(generateIconWithColor(clr));
            }
            return true; // 表示事件已处理
        }
        else if (event->type() == QEvent::MouseButtonPress)
        {
            // 处理单击事件
            QIcon icon = ui->toolButtonColor->icon();
            QPixmap pixmap = icon.pixmap(40, 40);
            QImage img = pixmap.toImage();
            QColor color;
                    color = img.pixelColor(20, 32);
            ui->textEdit->setTextColor(color);
            return true; // 表示事件已处理
        }
    }
    return QWidget::eventFilter(obj, event); // 交给基类处理其他事件
}

QIcon generateIconWithColor(const QColor& color)
{
    QPixmap pixmap(40, 40); // 图标大小
    pixmap.fill(Qt::transparent); // 背景透明

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制原始图标（例如一个"A"字图标）
    QFont font = painter.font();

    font.setPointSize(20);
    painter.setFont(font);
    painter.setPen(Qt::black);
    QRect textRect = pixmap.rect();
    textRect.moveTop(textRect.top() - 5); // 向上移动5个像素
    painter.drawText(textRect, Qt::AlignCenter, "A");

    // 在图标下方绘制颜色矩形
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    QRect rect(5, 30, 30, 5); // 矩形位置和大小
    painter.drawRoundedRect(rect, 4, 4); // 圆角矩形

    return QIcon(pixmap);
}

void Diaries::on_lineEditFontSize_editingFinished()
{
    int nFontSize = ui->lineEditFontSize->text().toInt();
    //设置字号
    ui->textEdit->setFontPointSize( nFontSize );
}

void Diaries::on_fontComboBox_currentFontChanged(const QFont &f)
{
    QTextCharFormat format;
    format.setFontFamily(f.family());

    QTextCursor cursor = ui->textEdit->textCursor();

    cursor.mergeCharFormat(format);
    ui->textEdit->mergeCurrentCharFormat(format);
}

void Diaries::on_toolButtonSave_clicked()
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "警告", "无法打开文件: " + file.errorString());
        return;
    }
    QTextStream out(&file);
    out << ui->textEdit->toHtml();
    file.close();
    ui->textEdit->document()->setModified(false);

    QFile file2(globalFilePath+'/'+"diaries_seq.txt");
    if (!file2.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qWarning() << "无法打开文件";
        return;
    }
    QTextStream in(&file2);
    QString filecontent=in.readAll();
    if(!filecontent.contains(currentDate)){
        file2.seek(file2.size());
        QTextStream out(&file2);
        out << currentDate << '\n';
    }
    file2.close();

    QMessageBox::information(this, "提示", "文件已保存: " + filePath);
}


void Diaries::on_toolButtonPicture_clicked()
{
    // 打开文件选择对话框
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择图片"), "", tr("图片文件 (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty()) {
        // 获取文本编辑器中的光标
        QTextCursor cursor = ui->textEdit->textCursor();
        // 插入图片
        QImage image(fileName);
        int newWidth = 400;
        int newHeight = image.height() * newWidth / image.width();
        QImage scaledImage = image.scaled(newWidth, newHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        // 将缩放后的图像转换为 base64 编码的字符串
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        scaledImage.save(&buffer, "PNG"); // 选择适当的格式，例如 PNG
        QString imageStr = QString::fromLatin1(byteArray.toBase64().data());
        QTextImageFormat imageFormat;
        imageFormat.setName("data:image/png;base64," + imageStr);
        imageFormat.setWidth(newWidth);
        imageFormat.setHeight(newHeight);
        cursor.insertImage(imageFormat);
    }
}

