#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

class Tile
{
public:
    cv::Rect boundingBox;
    double colorPercentage;

    Tile(const cv::Rect &box) : boundingBox(box), colorPercentage(0.0) {}

    // Function to calculate color percentage for the tile
    void calculateColorPercentage(const cv::Mat &birdseye, const cv::Scalar &lowerBound, const cv::Scalar &upperBound)
    {
        cv::Mat mask;
        cv::Mat hsv;
        cv::cvtColor(birdseye(boundingBox), hsv, cv::COLOR_BGR2HSV);
        cv::inRange(hsv, lowerBound, upperBound, mask);

        int totalPixels = mask.rows * mask.cols;
        int colorPixels = cv::countNonZero(mask);
        colorPercentage = (static_cast<double>(colorPixels) / totalPixels) * 100.0;
    }
};

void colorDetection(const cv::Mat &input, const cv::Scalar &lowerBound, const cv::Scalar &upperBound, std::vector<Tile> &tiles)
{
    cv::Mat hsv;
    cv::cvtColor(input, hsv, cv::COLOR_BGR2HSV);

    cv::Mat mask;
    cv::inRange(hsv, lowerBound, upperBound, mask);

    // Find contours in the mask
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Process each detected region
    for (const auto &contour : contours)
    {
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

int main()
{
    cv::VideoCapture cap(0); // Open the default camera (index 0)

    if (!cap.isOpened())
    {
        std::cerr << "Error opening the video source." << std::endl;
        return -1;
    }

    cv::namedWindow("Video Input", cv::WINDOW_NORMAL);
    cv::namedWindow("Bird's Eye View", cv::WINDOW_NORMAL);
    cv::namedWindow("Color Mask", cv::WINDOW_NORMAL);

    // Define source points (quadrilateral in the input image)
    std::vector<cv::Point2f> srcPoints;
    srcPoints.push_back(cv::Point2f(150, 10));  // Top-left
    srcPoints.push_back(cv::Point2f(400, 10));  // Top-right
    srcPoints.push_back(cv::Point2f(550, 200)); // Bottom-right
    srcPoints.push_back(cv::Point2f(50, 200));  // Bottom-left

    // Define destination points (rectangle in the output image)
    std::vector<cv::Point2f> dstPoints;
    dstPoints.push_back(cv::Point2f(100, 0));   // Top-left
    dstPoints.push_back(cv::Point2f(500, 0));   // Top-right
    dstPoints.push_back(cv::Point2f(500, 300)); // Bottom-right
    dstPoints.push_back(cv::Point2f(100, 300)); // Bottom-left

    cv::Mat perspectiveMatrix = cv::getPerspectiveTransform(srcPoints, dstPoints);

    while (true)
    {
        cv::Mat frame, birdseye;
        if (!cap.read(frame))
        {
            std::cerr << "Error capturing frame." << std::endl;
            break;
        }

        // Apply perspective transformation to get bird's-eye view
        cv::warpPerspective(frame, birdseye, perspectiveMatrix, frame.size());

        cv::Scalar lowerRed1 = cv::Scalar(0, 100, 100);  // Lower bound for red (first range)
        cv::Scalar upperRed1 = cv::Scalar(10, 255, 255); // Upper bound for red (first range)

        cv::Scalar lowerRed2 = cv::Scalar(160, 100, 100); // Lower bound for red (second range)
        cv::Scalar upperRed2 = cv::Scalar(180, 255, 255); // Upper bound for red (second range)

        cv::Scalar lowerBlue1 = cv::Scalar(100, 100, 100);
        cv::Scalar upperBlue1 = cv::Scalar(120, 255, 255);
        std::vector<Tile> redTiles, blueTiles;
        double colorPercentage;
        colorDetection(frame, lowerRed1, upperRed1, redTiles);
        colorDetection(frame, lowerBlue1, upperBlue1, blueTiles);
        for (const auto &tile : redTiles)
        {
            // Draw bounding box on the original frame
            cv::rectangle(frame, tile.boundingBox, cv::Scalar(0, 0, 255), 2); // Red color for bounding box

            // Display color percentage on the frame
            std::string text = "Red Percentage: " + std::to_string(tile.colorPercentage) + "%";

            // Print the color percentage to the console
            std::cout << text << std::endl;
        }
        for (const auto &tile : blueTiles)
        {
            // Draw bounding box on the original frame
            cv::rectangle(frame, tile.boundingBox, cv::Scalar(255, 0, 0), 2); // Blue color for bounding box

            // Display color percentage on the frame
            std::string text = "Blue Percentage: " + std::to_string(tile.colorPercentage) + "%";

            // Print the color percentage to the console
            std::cout << text << std::endl;
        }

        cv::imshow("Video Input", frame);
        cv::imshow("Bird's Eye View", birdseye);

        if (cv::waitKey(1) == 27)
        { // Exit when the 'Esc' key is pressed
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}
