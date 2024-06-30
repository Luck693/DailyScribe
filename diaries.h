#ifndef DIARIES_H
#define DIARIES_H

#include <QWidget>
#include <QTextCharFormat>
#include <QColorDialog>
namespace Ui {
class Diaries;
}

class Diaries : public QWidget
{
    Q_OBJECT

public:
    explicit Diaries(QWidget *parent = nullptr);
    ~Diaries();
    bool eventFilter(QObject *obj, QEvent *event) override;
private slots:

    void on_toolButtonBold_clicked();

    void on_textEdit_cursorPositionChanged();

    void on_toolButtonItalic_clicked();

    void on_toolButtonUnderline_clicked();

    void on_lineEditFontSize_editingFinished();


    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_toolButtonSave_clicked();

    void on_toolButtonPicture_clicked();

private:
    Ui::Diaries *ui;
    QString filePath;
    QString currentDate;
protected:
    void closeEvent(QCloseEvent *event) override;
};
QIcon generateIconWithColor(const QColor& color);
#endif // DIARIES_H
