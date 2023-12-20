#include "colordetection.h"
#include "birdview.h"


Tile::Tile(const cv::Rect& box) : boundingBox(box), colorPercentage(0.0) {}

void Tile::calculateColorPercentage(const cv::Mat& birdseye, const cv::Scalar& lowerBound, const cv::Scalar& upperBound) {
    cv::Mat mask, hsv;
    cv::cvtColor(birdseye(boundingBox), hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, lowerBound, upperBound, mask);

    //buraya bak
    int totalPixels = mask.rows * mask.cols;
    int colorPixels = cv::countNonZero(mask);
    colorPercentage = (static_cast<double>(colorPixels) / totalPixels) * 100.0;
}

colordetection::colordetection() {
    // constructor
}

void colordetection::colorDetection(const cv::Mat& input, const cv::Scalar& lowerBound, const cv::Scalar& upperBound, std::vector<Tile>& tiles) {
    cv::Mat hsv;
    cv::cvtColor(input, hsv, cv::COLOR_BGR2HSV);

    cv::Mat mask;
    cv::inRange(hsv, lowerBound, upperBound, mask);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) {
        // Calculate bounding box for each contour
        cv::Rect boundingBox = cv::boundingRect(contour);

        // Create a Tile object for each detected tile
        Tile tile(boundingBox);

        // Calculate color percentage for the tile
        tile.calculateColorPercentage(input, lowerBound, upperBound);

        // Add the tile to the vector
        tiles.push_back(tile);
    }
}


