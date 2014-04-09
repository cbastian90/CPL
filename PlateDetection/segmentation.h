#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <opencv2/opencv.hpp>
#include "hog.h"

class segmentation
{
public:
    segmentation();
    ~segmentation();
    //PUBLIC ATTRIBUTES
    cv::Mat _M_imgInput;
    cv::Mat _M_imgGray;         // OJO para futuro inmediato: volverla variable privada y generar sus accesores
    cv::Mat _M_imgSobel;
    cv::Mat _M_imgThreshold;
    cv::Mat _M_SE17x3;          //definira un elemento estructurante tipo rectangulo de dimensiones 17anchox3alto

    cv::Mat _M_Mask;
    cv::Mat _M_imgCenters;

    //ATTRIBUTES FOR FIND POSIBLES PLATES
    std::vector< std::vector <cv::Point> > _VVP_Contours;    //_VVP vector de vector de puntos
    std::vector<cv::RotatedRect > _VRR_Rects;
    cv::RotatedRect RR_MinAreaRect;

    //ATTRIBUTES FOR SAVE HOUG DESCRIPTORS.
    Hog _HogFeactures;

    //PUBLIC METHODS
    void ConvertImageToGray(cv::Mat M_input);

    void FindVerticalLines();

    void Display_CV_MAT_8();

    void FindContoursOfPossiblePlates();

    bool DetectRegionsVerifyPlateSizes(cv::RotatedRect RR_candidatesdf, int I_avgWidthPlate, int I_avgHeight);

    void NewClosestCroppingRegion();

    void CheckNewClosestCroppingRegion();

private:

};

#endif // SEGMENTATION_H
