#include "bWidgets.h"
#include "mainWindow.h"
#include "mineField.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    mainWindow b;
    b.show();

    return a.exec();
}
