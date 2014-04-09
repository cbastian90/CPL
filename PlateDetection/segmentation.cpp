#include "segmentation.h"
#include <QDebug>

segmentation::segmentation()
{
}

segmentation::~segmentation()
{
   //_M_imgSobel.~Mat();
   //_M_Mask.~Mat();
   //_VRR_Rects.~vector();
   //_VVP_Contours.~vector();
    _M_imgCenters.~Mat();
    _M_imgGray.~Mat();
    _M_SE17x3.~Mat();
    _M_imgThreshold.~Mat();
    _M_imgInput.~Mat();
    cv::destroyAllWindows();
    cv::destroyAllWindows();
}

/// ****************************************************
/// \brief segmentation::ConvertImageToGray
/// \param M_input
/// en este metodo se convierte la imagen
/// a escala de grises.
/// ****************************************************
void segmentation::ConvertImageToGray(cv::Mat M_input)
{
    if(M_input.data!=NULL)
    {
        M_input.copyTo(_M_imgInput);//se saca una copia a la imagen de entrada para hacer procesamiento sobre la copia.
        M_input.copyTo(_M_imgCenters);//se saca una copia a la imagen de entrada con el fin de pintar sobre esta los centros de las posibles placas encontradas
        if(M_input.channels()==1)
        {
            M_input.copyTo(_M_imgGray);
        }
        else
        {
            cv::cvtColor(M_input,_M_imgGray,CV_BGR2GRAY);
        }
        cv::blur(_M_imgGray,_M_imgGray,cv::Size(3,3));
        M_input.~Mat();
    }
}

void segmentation::FindVerticalLines()
{
    if(_M_imgGray.data)
    {
        //Sobel filter in order to find vertical edges on a gray image
        cv::Sobel(_M_imgGray,_M_imgSobel,CV_8U,1,0,3,1,0);
        //Threshold filter to obtain a binary image with a threshold obtained through Otsu´s method
        cv::threshold(_M_imgSobel,_M_imgThreshold,0,255,CV_THRESH_OTSU+CV_THRESH_BINARY);
        //Define the 17x3 structural element
        _M_SE17x3=cv::getStructuringElement(cv::MORPH_RECT,cv::Size(19,3));
        //Aplay a close morphological operation
        cv::morphologyEx(_M_imgThreshold,_M_imgThreshold,CV_MOP_CLOSE,_M_SE17x3);
        cv::erode(_M_imgThreshold,_M_imgThreshold,_M_SE17x3);
        //cv::morphologyEx(_M_imgThreshold,_M_imgThreshold,CV_MOP_TOPHAT,_M_SE17x3);
        cv::dilate(_M_imgThreshold,_M_imgThreshold,_M_SE17x3);
        cv::Mat _M_SE3x5=cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,9));
        cv::dilate(_M_imgThreshold,_M_imgThreshold,_M_SE3x5);
        cv::dilate(_M_imgThreshold,_M_imgThreshold,_M_SE17x3);
        cv::imshow("imagen umbral",_M_imgThreshold);
        _M_SE3x5.~Mat();
    }
}

void segmentation::FindContoursOfPossiblePlates()
{
    cv::findContours(_M_imgThreshold.clone(),_VVP_Contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);//_M_imgThreshold.clone()
    //Start to iterate to each contour found
    std::vector<std::vector<cv::Point> >::iterator itc= _VVP_Contours.begin();
    //Remove patch that has no inside limits of aspect ratio and area.
    while(itc!=_VVP_Contours.end())
    {
        //create bounding rect of object
        RR_MinAreaRect=cv::minAreaRect(cv::Mat(*itc));

        if(!DetectRegionsVerifyPlateSizes(RR_MinAreaRect,20,20))
        {
            itc=_VVP_Contours.erase(itc);
        }
        else
        {
            ++itc;
            _VRR_Rects.push_back(RR_MinAreaRect);
        }
    }

    qDebug()<<"asdf"<<_VRR_Rects.size();
    for(int p=0;p<_VRR_Rects.size();p++)
    {
        cv::circle(_M_imgCenters,_VRR_Rects[p].center,5,cv::Scalar(0,255,0),-1);
        cv::Point2f vertices[4];
        _VRR_Rects[p].points(vertices);
        for (int i = 0; i < 4; i++)
            line(_M_imgCenters, vertices[i], vertices[(i+1)%4],cv::Scalar(255,255,0),3);
    }
    cv::imshow("centros",_M_imgCenters);
}

void segmentation::NewClosestCroppingRegion()
{
    //qDebug()<<"1";
    qDebug()<<_VRR_Rects.size();
    for(int i=0; i< _VRR_Rects.size(); i++)
    {
        cv::circle(_M_imgCenters,_VRR_Rects[1].center,5,cv::Scalar(0,255,0),-1);
        cv::imshow("centers",_M_imgCenters);
        //cv::waitKey(1000);
        float F_minSize=(_VRR_Rects[i].size.width < _VRR_Rects[i].size.height) ? _VRR_Rects[i].size.width : _VRR_Rects[i].size.height;
        F_minSize=F_minSize*0.5f;
        //initialize rand and get 5 points around center for floodfill algorithm
        srand(time_t(NULL));//OJO ESTA LINEA NO QUEDO IGUAL AL LIBRO;PUEDE HABER PROBLEMAS AL GENERAR LAS SEMILLAS POR EL "time"
        //inizialize floodfill parameters and variables
        /*if(_M_Mask.data)
        {
            _M_Mask.~Mat();
        }*/
        _M_Mask.create(_M_imgInput.rows + 2, _M_imgInput.cols +2 ,CV_8UC1);
        _M_Mask=cv::Scalar::all(0);
        int I_LowDiff = 30;
        int I_UpDiff  = 30;
        int I_Connectivity = 4;
        int I_NewMaskValue = 255;
        int I_NumberOfSeeds = 10;
        int I_Flags= I_Connectivity + (I_NewMaskValue << 8) + CV_FLOODFILL_FIXED_RANGE+ CV_FLOODFILL_MASK_ONLY;
        cv::Rect ccomp;
        for(int j=0; j<I_NumberOfSeeds;j++)
        {
            cv::Point P_seed;
            P_seed.x=_VRR_Rects[i].center.x + rand()%(int)F_minSize-(F_minSize/2);
            P_seed.y=_VRR_Rects[i].center.y + rand()%(int)F_minSize-(F_minSize/2);
            //cv::circle()
            int I_Area = cv::floodFill(_M_imgInput,_M_Mask,P_seed,cv::Scalar(255,0,0),&ccomp,cv::Scalar(I_LowDiff,I_LowDiff,I_LowDiff),cv::Scalar(I_UpDiff,I_UpDiff,I_UpDiff),I_Flags);
            //qDebug()<<"area"<<I_Area;

        }
    }

}

void segmentation::CheckNewClosestCroppingRegion()
{

    std::vector<cv::Point> VP_PointsOfInterest;
    cv::Mat_<uchar>::iterator itMask =_M_Mask.begin<uchar>();
    cv::Mat_<uchar>::iterator end    =_M_Mask.end<uchar>();
    std::cout<<"salio de donde creo que esta el error";
    for(;itMask!=end;++itMask)
    {
        if(*itMask==255)
        {
            VP_PointsOfInterest.push_back(itMask.pos());
        }
    }
    cv::RotatedRect minRect= cv::minAreaRect(VP_PointsOfInterest);

    if(DetectRegionsVerifyPlateSizes(minRect,115,54))
    {
        //get rotation matix
        float r= minRect.size.width / (float)minRect.size.height;
        float angle= minRect.angle;
        if(r<1)
        {
            angle=90+angle;
        }
        cv::Mat rotmat=cv::getRotationMatrix2D(minRect.center,angle,1);
        //create and rotate image
        cv::Mat img_rotated;
        cv::warpAffine(_M_imgInput,img_rotated,rotmat,_M_imgInput.size(),CV_INTER_CUBIC);
        cv::Size rect_size=minRect.size;
        if(r<1)
        {
            std::swap(rect_size.width,rect_size.height);
        }
        cv::Mat img_crop;
        cv::getRectSubPix(img_rotated,rect_size,minRect.center,img_crop);
        cv::imshow("img_crop",img_crop);
        //cv::waitKey(1000);

    }

}

bool segmentation::DetectRegionsVerifyPlateSizes(cv::RotatedRect RR_candidates,int I_avgWidthPlate=115,int I_avgHeight=54)
{
    float F_Error=0.6f;
    //colombian Plate size: I_avgWidthPlate  x  I_avgHeight
    float aspect = I_avgWidthPlate/I_avgHeight;// I_avgWidthPlate/I_avgHeight;4.7272f
    //set min and max area.
    int I_minThresholdArea = 50 *aspect* 50;
    int I_maxThresholdArea = 160*aspect* 160;
    //Get only patches that match to a respect ratio.
    float F_RatioMin= aspect-aspect*F_Error;
    float F_RatioMax= aspect+aspect*F_Error;

    int I_PlateArea=RR_candidates.size.area();//RR_candidates.size.height * RR_candidates.size.width
    float F_Ratio=(float)RR_candidates.size.width / (float)RR_candidates.size.height;
    F_Ratio=1/F_Ratio;

    //if(( I_PlateArea < I_minThresholdArea || I_PlateArea > I_maxThresholdArea ) || ( F_Ratio < F_RatioMin || F_Ratio > F_RatioMax ) )
    if(RR_candidates.size.width<I_avgWidthPlate || RR_candidates.size.height<I_avgHeight)
    //if(RR_candidates.size.width<RR_candidates.size.height )//|| RR_candidates.size.width<I_avgWidthPlate || RR_candidates.size.height<I_avgHeight
    {
        return false;
    }else
    {
        return true;
    }
}

void segmentation::Display_CV_MAT_8()
{
    cv::imshow("imagenGris",_M_imgGray);
    cv::imshow("imagen sobel",_M_imgSobel);
    cv::imshow("imagenUmbralLineas verticales",_M_imgThreshold);
    cv::imshow("input",_M_imgInput);
    cv::imshow("mascara",_M_Mask);


}


