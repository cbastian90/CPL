#ifndef DISPLAYQTGUI_H
#define DISPLAYQTGUI_H

#include <opencv2/opencv.hpp>

class DisplayQTGUI
{
public:
    DisplayQTGUI();
    void Display_CV_MAT_8_Gray(cv::Mat M_imgGray);
    void Display_CV_MAT_8_RGB(cv::Mat M_imgBGR);
};

#endif // DISPLAYQTGUI_H
