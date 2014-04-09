#include "hog.h"

Hog::Hog()
{
}

Hog::~Hog()
{
}

/***************************************************************************************************
Metodos
Autor: Alexander Gómez villa - Sebastian Guzman Obando - German Diez Valencia
Descripcion: Halla los descriptores HOG de la imagen ingresada y los devuelve en un vector

***************************************************************************************************/


vector<float> Hog::getHOG(Mat &img, Size winSize, Size blockStride)
{
    resize(img, img,winSize);
    cvtColor(img, img, CV_RGB2GRAY);

    HOGDescriptor d(winSize, //winSize
     Size(16,16), //blocksize
     blockStride, //blockStride,
     Size(8,8), //cellSize,
     9, //nbins,
     0, //derivAper,
     -1, //winSigma,
     0, //histogramNormType,
     0.2, //L2HysThresh,
     0); //gammal correction,

    d.compute( img, descriptorsValues, cv::Size(0,0), cv::Size(0,0), locations);
    return descriptorsValues;
}
