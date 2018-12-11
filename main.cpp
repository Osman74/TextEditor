#include "mainwindow.h"
#include <QApplication>
#include "texteditor.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TextEditor TE;
    TE.resize(700, 500);
    TE.showMaximized();
    return a.exec();
}
