#include "displayqtgui.h"

DisplayQTGUI::DisplayQTGUI()
{
}

void DisplayQTGUI::Display_CV_MAT_8_Gray(cv::Mat M_imgGray)
{
    cv::imshow("imagenGris",M_imgGray);
}

void DisplayQTGUI::Display_CV_MAT_8_RGB(cv::Mat M_imgBGR)
{
    cv::imshow("imagenGris",M_imgBGR);
}
