#ifndef HOG_H
#define HOG_H
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>

using namespace cv;
using namespace std;

class Hog
{
public:
    Hog();
    ~Hog();


    HOGDescriptor H_featHog;
    std::vector<float> descriptorsValues;
    std::vector<Point> locations;


    std::vector<float> getHOG(cv::Mat& img,cv::Size winSize,cv::Size blockStride);


};

#endif // HOG_H
