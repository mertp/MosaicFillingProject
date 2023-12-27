#ifndef COLORDETECTION_H
#define COLORDETECTION_H

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core.hpp>
#include <vector>
#include <iostream>

class Tile {
public:
    cv::Rect boundingBox;
    double colorPercentage;

    Tile(const cv::Rect& box);

    void calculateColorPercentage(const cv::Mat& birdseye, const cv::Scalar& lowerBound, const cv::Scalar& upperBound);
};

class colordetection {

public:
    colordetection(); // Ba�lat�c� ekleniyor.
    void colorDetection(const cv::Mat& input, const cv::Scalar& lowerBound, const cv::Scalar& upperBound, std::vector<Tile>& tiles);
};
#endif // COLORDETECTION_H

