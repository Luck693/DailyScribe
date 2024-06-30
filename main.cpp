#include "form.h"
#include "page_login.h"
#include "jottings.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Page_Login w;
    w.show();
    return a.exec();
}
