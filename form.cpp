#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Form)
    , inkWidget(new InkWidget(this))  // 初始化墨迹绘制部件
    , quoteTimer(new QTimer(this))
{
    ui->setupUi(this);

    // 设置初始页面为 Totings 页面
    ui->stackedWidget->setCurrentIndex(1);
    inkWidget->setGeometry(this->rect());
    inkWidget->raise();
    // 安装事件过滤器
    ui->stackedWidget->installEventFilter(this);

    // Connect buttons to change pages from totingsPage
    connect(ui->btnToDiariesFromTotings, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(1); });
    connect(ui->btnToRecollectionsFromTotings, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(2); });
    connect(ui->btnToSettingsFromTotings, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(3); });

    // Connect buttons to change pages from diariesPage
    connect(ui->btnToTotingsFromDiaries, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(0); });
    connect(ui->btnToRecollectionsFromDiaries, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(2); });
    connect(ui->btnToSettingsFromDiaries, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(3); });

    // Connect buttons to change pages from recollectionsPage
    connect(ui->btnToTotingsFromRecollections, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(0); });
    connect(ui->btnToDiariesFromRecollections, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(1); });
    connect(ui->btnToSettingsFromRecollections, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(3); });

    // Connect buttons to change pages from settingsPage
    connect(ui->btnToTotingsFromSettings, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(0); });
    connect(ui->btnToDiariesFromSettings, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(1); });
    connect(ui->btnToRecollectionsFromSettings, &QPushButton::clicked, this, [=]() { ui->stackedWidget->setCurrentIndex(2); });
    diary = new Diaries(ui->frame_5);
    jotting = new Jottings(ui->frame_8);
    recollection = new Recollections(ui->frame_6);
    setting = new Settings(ui->frame_7);
    // Load quotes from the file
    loadQuotes();

    // Setup the timer to update the quote every 30 seconds
    connect(quoteTimer, &QTimer::timeout, this, &Form::updateQuote);
    quoteTimer->start(30000);  // 30 seconds interval

    // Display the first quote immediately
    updateQuote();
}

Form::~Form()
{
    delete ui;
    delete diary;
    delete jotting;
    delete recollection;
    delete setting;
}
void Form::closeEvent(QCloseEvent *event){
    jotting->close();
    diary->close();  // 手动触发 Diaries 的 closeEvent
    event->accept();
}
bool Form::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseMove) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        inkWidget->mouseMoveEvent(mouseEvent);  // 将事件传递给墨迹绘制部件
        return true; // 拦截事件
    }
    return QWidget::eventFilter(watched, event);
}

void Form::loadQuotes()
{
    QFile file(":/new/prefix1/quote.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open quote.txt";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.trimmed().isEmpty()) {
            quotes << line.trimmed();
        }
    }
    file.close();
}

void Form::updateQuote()
{
    if (quotes.isEmpty()) {
        ui->labelQuote->setText("今日名言：No quotes available.");
        return;
    }

    int index = QRandomGenerator::global()->bounded(quotes.size());
    ui->labelQuote->setText("今日名言：" + quotes.at(index));
}
