#ifndef JOTTINGS_H
#define JOTTINGS_H

#include <QWidget>
#include <QToolButton>
#include <QMenu>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QList>
#include <QPushButton>

namespace Ui {
class Jottings;
}

class Jottings : public QWidget
{
    Q_OBJECT

public:
    explicit Jottings(QWidget *parent = nullptr);
    ~Jottings();

private slots:
    void onAddButtonClicked();
    void onDeleteTextBox();
    void handleEditingFinished();
    void onSaveButtonClicked(); // 新添加的保存按钮槽函数声明

private:
    Ui::Jottings *ui;
    QVBoxLayout *mainLayout;
    QVBoxLayout *layout;
    QToolButton *addButton;
    QMenu *contextMenu;
    QLineEdit *currentTextBox;
    QList<QLineEdit *> textBoxes;  // 确保声明 textBoxes
    QVector<QComboBox*> comboBoxes;
    QString filePath;
    QString currentDate;
    void createTextBox(const QString &text = "", int comboIndex = 0);
    void createComboBox(QComboBox *comboBox, QLabel *iconBox);
    void closeEvent(QCloseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void loadTextBoxes();

};

#endif // JOTTINGS_H
