#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

void colorDetection(const cv::Mat& input, const cv::Scalar& lowerBound, const cv::Scalar& upperBound, double& colorPercentage) {
    cv::Mat hsv;
    cv::cvtColor(input, hsv, cv::COLOR_BGR2HSV);

    cv::Mat mask;
    cv::inRange(hsv, lowerBound, upperBound, mask);

    cv::imshow("Color Mask", mask);

    // Calculate color percentage
    int totalPixels = input.rows * input.cols;
    int colorPixels = cv::countNonZero(mask);
    colorPercentage = (static_cast<double>(colorPixels) / totalPixels) * 100.0;
}

int main() {
    cv::VideoCapture cap(0); // Open the default camera (index 0)

    if (!cap.isOpened()) {
        std::cerr << "Error opening the video source." << std::endl;
        return -1;
    }

    cv::namedWindow("Video Input", cv::WINDOW_NORMAL);
    cv::namedWindow("Bird's Eye View", cv::WINDOW_NORMAL);
    cv::namedWindow("Color Mask", cv::WINDOW_NORMAL);

    // Define source points (quadrilateral in the input image)
    std::vector<cv::Point2f> srcPoints;
    srcPoints.push_back(cv::Point2f(150, 100));  // Top-left
    srcPoints.push_back(cv::Point2f(400, 100));  // Top-right
    srcPoints.push_back(cv::Point2f(550, 300));  // Bottom-right
    srcPoints.push_back(cv::Point2f(50, 300));   // Bottom-left

    // Define destination points (rectangle in the output image)
    std::vector<cv::Point2f> dstPoints;
    dstPoints.push_back(cv::Point2f(100, 0));    // Top-left
    dstPoints.push_back(cv::Point2f(500, 0));    // Top-right
    dstPoints.push_back(cv::Point2f(500, 300));  // Bottom-right
    dstPoints.push_back(cv::Point2f(100, 300));  // Bottom-left

    cv::Mat perspectiveMatrix = cv::getPerspectiveTransform(srcPoints, dstPoints);

    while (true) {
        cv::Mat frame, birdseye;
        if (!cap.read(frame)) {
            std::cerr << "Error capturing frame." << std::endl;
            break;
        }

        // Apply perspective transformation to get bird's-eye view
        cv::warpPerspective(frame, birdseye, perspectiveMatrix, frame.size());

        cv::Scalar lowerBlue = cv::Scalar(0, 0, 0); // Adjust as needed
        cv::Scalar upperBlue = cv::Scalar(105, 105, 105); // Adjust as needed
        double colorPercentage;
        colorDetection(frame, lowerBlue, upperBlue,colorPercentage);

         std::string text = "Black color Percentage: " + std::to_string(colorPercentage) + "%";
        cv::putText(frame, text, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 0), 2, cv::LINE_AA);

        cv::imshow("Video Input", frame);
        //cv::imshow("Bird's Eye View", birdseye);

        if (cv::waitKey(1) == 27) { // Exit when the 'Esc' key is pressed
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}
