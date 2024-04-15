#include "EmojiTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EmojiTest w;
    w.show();
    return a.exec();
}
