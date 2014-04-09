#include "platedetection.h"
#include "ui_platedetection.h"

PlateDetection::PlateDetection(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlateDetection)
{

    a=cv::imread("C:/PDI/ImgVehiculosPlaca/palmostro5.png");
    seg.ConvertImageToGray(a);
    seg.FindVerticalLines();
    seg.FindContoursOfPossiblePlates();
    //seg.NewClosestCroppingRegion();
    //seg.CheckNewClosestCroppingRegion();
    //seg.Display_CV_MAT_8();
    ui->setupUi(this);
}

PlateDetection::~PlateDetection()
{

    delete ui;
    //seg.~segmentation();
    a.~Mat();

}
