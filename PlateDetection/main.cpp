#include "platedetection.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlateDetection w;
    w.show();
    
    return a.exec();
}
