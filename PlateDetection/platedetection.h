#ifndef PLATEDETECTION_H
#define PLATEDETECTION_H

#include <QMainWindow>
#include <segmentation.h>

namespace Ui {
class PlateDetection;
}

class PlateDetection : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit PlateDetection(QWidget *parent = 0);
    ~PlateDetection();
    segmentation seg;
    cv::Mat a;
    
private:
    Ui::PlateDetection *ui;
};

#endif // PLATEDETECTION_H
