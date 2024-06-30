#include "DiaryDialog.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

DiaryDialog::DiaryDialog(const QString &filePath, QWidget *parent)
    : QWidget(parent), textEdit(new QTextEdit(this)) {

    QVBoxLayout *layout = new QVBoxLayout(this);

    // 设置 QTextEdit 为只读模式
    textEdit->setReadOnly(true);

    setMinimumSize(400, 300);
    setMaximumSize(800, 600);
    // 加载 HTML 文件
    loadHtmlFile(filePath);

    layout->addWidget(textEdit);
    setLayout(layout);
    adjustSize();
}

void DiaryDialog::loadHtmlFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot read file %1:\n%2.")
                                                    .arg(filePath, file.errorString()));
        return;
    }

    QTextStream in(&file);
    QString htmlContent = in.readAll();
    textEdit->setHtml(htmlContent);
}
