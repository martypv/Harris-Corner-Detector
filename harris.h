#ifndef HARRIS_H
#define HARRIS_H

#define DATA_DIRECTORY "/home/paul/Desktop/Comp 490/HarrisCorners/"

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
//#include <opencv2/nonfree/features2d.hpp>
#include <QLabel>
#include <QMainWindow>
#include <QtCore>
#include <QPushButton>

using namespace cv;
using namespace std;

class harris
{
public:
    harris();
    void setLocalMaxWindowSize(int size);
    Mat detect(const Mat& image);
    Mat detect2(const Mat& image);
    Mat getCornerMap(double qualityLevel);
    Mat getCornerMap2(double qualityLevel);
    void getCorners(std::vector<cv::Point> &points, double qualityLevel);
    void getCorners2(std::vector<cv::Point> &points, double qualityLevel);
    void getCorners(std::vector<cv::Point> &points, const cv::Mat& cornerMap);
    Mat drawOnImage(cv::Mat &image, const std::vector<cv::Point> &points);
    QImage convertToQImage();
    void displayImage(QLabel &location);
    QImage convertToQImage2();
    void displayImage2(QLabel &location);
    QImage convertToQImage3();
    void displayImage3(QLabel &location);
    void doCorners();
    void compare(Mat &image, Mat &image2);
    bool match(Mat &image, Mat &image2);
    bool match2(const std::vector<cv::Point> &points1, const std::vector<cv::Point> &points2,int size1,int size2);

private:

    Mat src, src2, good, display, display2, display3, image,gray,corner1,corner2,out,more,buff;

    Mat im1,im2,out1,out2,gray1,gray2;

    Mat cornerStrength, cornerStrength2, cornerTh, cornerTh2, localMax, localMax2, kernel;

    int neighbourhood;
    int aperture;
    double k;
    double maxStrength;
    double threshold;
    int nonMaxSize;




};

#endif // HARRIS_H
