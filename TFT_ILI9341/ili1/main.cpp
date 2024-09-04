#include "bonegui.h"

#include <QApplication>
#include <unistd.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    boneGUI w;
    w.show();
    return a.exec();
}
