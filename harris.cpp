#include "harris.h"
#include <iostream>
#include <tgmath.h>
#include <math.h>


harris::harris() : neighbourhood(5), aperture(5), k(0.01), maxStrength(0.0), threshold(0.01), nonMaxSize(3)
{
     setLocalMaxWindowSize(nonMaxSize);

}

void harris::setLocalMaxWindowSize(int size){
    nonMaxSize= size;
    kernel.create(nonMaxSize,nonMaxSize,CV_8U);
}

Mat harris::detect(const Mat& image){
    // Harris computation
    cv::cornerHarris(image,cornerStrength,
               neighbourhood,// neighborhood size
               aperture,     // aperture size
               k);           // Harris parameter

    // internal threshold computation
    double minStrength; // not used
    cv::minMaxLoc(cornerStrength,&minStrength,&maxStrength);

    // local maxima detection
    cv::Mat dilated;  // temporary image
    cv::dilate(cornerStrength,dilated,cv::Mat());
    cv::compare(cornerStrength,dilated,localMax,cv::CMP_EQ);
    return localMax;

}

Mat harris::detect2(const Mat& image){
    // Harris computation
    cv::cornerHarris(image,cornerStrength2,
               neighbourhood,// neighborhood size
               aperture,     // aperture size
               k);           // Harris parameter

    // internal threshold computation
    double minStrength; // not used
    cv::minMaxLoc(cornerStrength2,&minStrength,&maxStrength);

    // local maxima detection
    cv::Mat dilated;  // temporary image
    cv::dilate(cornerStrength2,dilated,cv::Mat());
    cv::compare(cornerStrength2,dilated,localMax2,cv::CMP_EQ);
    return localMax2;

}



Mat harris::getCornerMap(double qualityLevel){

    cv::Mat cornerMap;

    // thresholding the corner strength
    threshold= qualityLevel*maxStrength;
    cv::threshold(cornerStrength,cornerTh,threshold,255,cv::THRESH_BINARY);

    // convert to 8-bit image
    cornerTh.convertTo(cornerMap,CV_8U);

    // non-maxima suppression
    cv::bitwise_and(cornerMap,localMax,cornerMap);

    return cornerMap;
}

Mat harris::getCornerMap2(double qualityLevel){

    cv::Mat cornerMap;

    // thresholding the corner strength
    threshold= qualityLevel*maxStrength;
    cv::threshold(cornerStrength2,cornerTh2,threshold,255,cv::THRESH_BINARY);

    // convert to 8-bit image
    cornerTh2.convertTo(cornerMap,CV_8U);

    // non-maxima suppression
    cv::bitwise_and(cornerMap,localMax2,cornerMap);

    return cornerMap;
}



void harris::getCorners(std::vector<cv::Point> &points, double qualityLevel) {

    // Get the corner map
    cv::Mat cornerMap= getCornerMap(qualityLevel);
    // Get the corners
    getCorners(points, cornerMap);

}

void harris::getCorners2(std::vector<cv::Point> &points, double qualityLevel) {

    // Get the corner map
    cv::Mat cornerMap= getCornerMap2(qualityLevel);
    // Get the corners
    getCorners(points, cornerMap);

}



void harris::getCorners(std::vector<cv::Point> &points, const cv::Mat& cornerMap) {

    // Iterate over the pixels to obtain all feature points
    for( int y = 0; y < cornerMap.rows; y++ ) {

        const uchar* rowPtr = cornerMap.ptr<uchar>(y);

        for( int x = 0; x < cornerMap.cols; x++ ) {

            // if it is a feature point
            if (rowPtr[x]) {

                points.push_back(cv::Point(x,y));

            }
        }
    }
}



Mat harris::drawOnImage(cv::Mat &image, const std::vector<cv::Point> &points) {

    std::vector<cv::Point>::const_iterator it= points.begin();
    cv::Scalar color = cv::Scalar(255,255,255);
    int radius = 3;
    int thickness = 2;

    // for all corners
    while (it!=points.end()) {

        // draw a circle at each corner location
        cv::circle(image,*it,radius,color,thickness);
        ++it;
    }

    return image;
}


QImage harris::convertToQImage(){
    cvtColor(src,display,CV_RGB2BGR);
    QImage img=QImage((const unsigned char*)(display.data),display.cols,display.rows,display.step,QImage::Format_RGB888);
   //  QImage img=QImage((const unsigned char*)(src.data),src.cols,src.rows,src.step,QImage::Format_RGB888);
    return img;
}


void harris::displayImage(QLabel &location){
    QImage img = convertToQImage();
    location.setPixmap(QPixmap::fromImage(img));
}

QImage harris::convertToQImage2(){
    cvtColor(src2,display2,CV_RGB2BGR);
    QImage img=QImage((const unsigned char*)(display2.data),display2.cols,display2.rows,display2.step,QImage::Format_RGB888);
   //  QImage img=QImage((const unsigned char*)(src.data),src.cols,src.rows,src.step,QImage::Format_RGB888);
    return img;
}


void harris::displayImage2(QLabel &location){
    QImage img = convertToQImage2();
    location.setPixmap(QPixmap::fromImage(img));
}

QImage harris::convertToQImage3(){
    cvtColor(good,display3,CV_RGB2BGR);
    QImage img=QImage((const unsigned char*)(display3.data),display3.cols,display3.rows,display3.step,QImage::Format_RGB888);
   //  QImage img=QImage((const unsigned char*)(src.data),src.cols,src.rows,src.step,QImage::Format_RGB888);
    return img;
}


void harris::displayImage3(QLabel &location){
    QImage img = convertToQImage3();
    location.setPixmap(QPixmap::fromImage(img));
}


void harris::doCorners(){

   image = imread("/home/paul/Desktop/Comp 490/HarrisCorners/build.jpg",1);
 //  image = imread("/home/paul/Desktop/Comp 490/HarrisCorners/church.jpg",1);
//    image.assignTo(src);

    cvtColor(image,corner1,CV_RGB2GRAY);
 //   cvtColor(theCorn,src,CV_GRAY2RGB);
 //    cvtColor(theCorn,more,CV_GRAY2RGB);

    cornerHarris(corner1,cornerStrength,neighbourhood,aperture,threshold);
    cv::threshold(cornerStrength,out,threshold,255,THRESH_BINARY_INV);


 //   cvtColor(out,src,CV_RGB2GRAY);
    cvtColor(out,src,CV_GRAY2RGB);



}

void harris::compare(Mat &image, Mat &image2){
    im1 = image;
    im2 = image2;

    cvtColor(im1,gray1,CV_RGB2GRAY);
    cvtColor(im2,gray2,CV_RGB2GRAY);

    corner1 = detect(gray1);
    corner2 = detect2(gray2);

    std::vector<cv::Point> pts1;
    std::vector<cv::Point> pts2;

    getCorners(pts1,2);
    getCorners2(pts2,0.2);
    out1 = drawOnImage(gray1,pts1);
    out2 = drawOnImage(gray2,pts2);

    int size1 = out1.rows * out1.cols;
    int size2 = out2.rows * out2.cols;

    bool check = match2(pts1,pts2,size1,size2);

    cvtColor(image,buff,CV_RGB2BGR);
    cvtColor(buff,good,CV_BGR2RGB);
    cvtColor(out1,src,CV_GRAY2RGB);
    cvtColor(out2,src2,CV_GRAY2RGB);


    if(check == true){
        cv::Scalar color = cv::Scalar(0,255,0);
        int radius = 40;
        int thickness = 20;
        cv::circle(image,cv::Point(image.cols/2,image.rows/2),radius,color,thickness);
    } else {
        cv::Scalar color = cv::Scalar(0,0,255);
        int radius = 40;
        int thickness = 20;
        cv::circle(image,cv::Point(image.cols/2,image.rows/2),radius,color,thickness);
    }


}

bool harris::match(Mat &image, Mat &image2){
    /*
    im1 = image;
    im2 = image2;

    cvtColor(im1,gray1,CV_RGB2GRAY);
    cvtColor(im2,gray2,CV_RGB2GRAY);

    corner1 = detect(gray1);
    corner2 = detect2(gray2);

    std::vector<cv::Point> pts1;
    std::vector<cv::Point> pts2;

    getCorners(pts1,0.2);
    getCorners2(pts2,0.2);

    return false;
*/
}
bool harris::match2(const std::vector<cv::Point> &points1, const std::vector<cv::Point> &points2, int size1, int size2){
//    std::vector<cv::Point>::const_iterator it= points1.begin();
//    std::vector<cv::Point>::const_iterator it2= points1.begin();
//    std::vector<cv::Point>::const_iterator it3= points2.begin();
//    std::vector<cv::Point>::const_iterator it4= points2.begin();

    int i = 0;
    int total = 0;
    int max = 0;
    int min = 0;
    int check = 0;
    float first [size1];
    float second [size2];

    if(size1 > size2){
        max = points1.max_size();
        min = points2.max_size();
    } else {
        max = points2.max_size();
        min = points1.max_size();
        check = 1;
    }


    for (std::vector<cv::Point>::const_iterator it= points1.begin(); it != points1.end(); ++it){
        for (std::vector<cv::Point>::const_iterator it2= points1.begin() + 1; it2 != points1.end(); ++it2){
            float xDelta = it2->x - it->x;
            float yDelta = it2->y - it->y;
            float dist = (xDelta * xDelta) + (yDelta * yDelta);
            first[i] = dist;
            cout << i << endl;
            i++;
        }
    }

    int j = 0;
    for (std::vector<cv::Point>::const_iterator it3= points2.begin(); it3 != points2.end(); ++it3){
        for (std::vector<cv::Point>::const_iterator it4= points2.begin() + 1; it4 != points2.end(); ++it4){
            float xDelta = it4->x - it3->x;
            float yDelta = it4->y - it3->y;
            float dist = (xDelta * xDelta) + (yDelta * yDelta);
            first[j] = dist;
            cout << j << endl;
            j++;
        }
    }

    if(check!=0){
        for(int x =0; x < max; x++){
            for(int y = 0; y < min; y++){
                if(first[x]==second[y]){
                    total++;
                }
            }
        }
    } else {
        for(int x =0; x < max; x++){
            for(int y = 0; y < min; y++){
                if(second[x]==first[y]){
                    total++;
                }
            }
        }
    }

    if(total >= (min/2)){
        return true;
    } else {
        return false;
    }


}















































