#include <opencv2/opencv.hpp>

int main() 
{
    //Reading image
    std::string path = "mosaic.jpeg";
    cv::Mat img = cv::imread(path);
    cv::Mat src_gray;
    cv::cvtColor(img, src_gray, cv::COLOR_BGR2GRAY);
    cv::imshow("Original Image", img);
    cv::waitKey(0);

    //Canny edge detection
    cv::Mat canny_img;
    cv::Canny(src_gray, canny_img, 150, 150);
    cv::imshow("Canny Image", canny_img);
    cv::waitKey(0);

    //Sobel edge detection
    int kernel_size = 3;
    cv::Mat sobelx, sobely, sobelxy;
    cv::Sobel(src_gray, sobelx, CV_64F, 1, 0, kernel_size);
    cv::Sobel(src_gray, sobely, CV_64F, 0, 1, kernel_size);
    cv::Sobel(src_gray, sobelxy, CV_64F, 1, 1, kernel_size);

    // Display Sobel edge detection images
    cv::imshow("SobelX", sobelx);
    cv::waitKey(0);
    cv::imshow("SobelY", sobely);
    cv::waitKey(0);
    cv::imshow("SobelXY", sobelxy);
    cv::waitKey(0);
    return 0;

}