#ifndef DIARYDIALOG_H
#define DIARYDIALOG_H

#include <QWidget>
#include <QString>

class QTextEdit;

class DiaryDialog : public QWidget {
    Q_OBJECT

public:
    explicit DiaryDialog(const QString &filePath, QWidget *parent = nullptr);

private:
    QTextEdit *textEdit;
    void loadHtmlFile(const QString &filePath);
};

#endif // DIARYDIALOG_H
