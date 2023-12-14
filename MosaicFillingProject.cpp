#include "MosaicFillingProject.h"
#include "birdview.h"
#include "colordetection.h"

cv::Scalar lowerRed1 = cv::Scalar(0, 100, 100);  // Lower bound for red (first range)
cv::Scalar upperRed1 = cv::Scalar(10, 255, 255); // Upper bound for red (first range)

cv::Scalar lowerRed2 = cv::Scalar(160, 100, 100); // Lower bound for red (second range)
cv::Scalar upperRed2 = cv::Scalar(180, 255, 255); // Upper bound for red (second range)

cv::Scalar lowerBlue1 = cv::Scalar(100, 100, 100);
cv::Scalar upperBlue1 = cv::Scalar(120, 255, 255);

int main() {
    birdview view;
    colordetection detect;

    string refImagePath = "/Users/Hp/Desktop/MosaicFillingProject/img4.jpg";
    Mat imReference = imread(refImagePath);
    if (imReference.empty()) {
        cerr << "Error: Reference image cannot be loaded." << endl;
        return -1;
    }

    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Error: Camera cannot be opened." << endl;
        return -1;
    }

    Mat frame, imReg, h;

    while (true) {
        cap >> frame;

        if (frame.empty()) {
            cerr << "End of video stream." << endl;
            break;
        }


        std::vector<Tile> redTiles, blueTiles;
        double colorPercentage;
        detect.colorDetection(frame, lowerRed1, upperRed1, redTiles);
        detect.colorDetection(frame, lowerBlue1, upperBlue1, blueTiles);
        for (const auto& tile : redTiles)
        {
            // Draw bounding box on the original frame
            cv::rectangle(frame, tile.boundingBox, cv::Scalar(0, 0, 255), 2); // Red color for bounding box

            // Display color percentage on the frame
            std::string text = "Red Percentage: " + std::to_string(tile.colorPercentage) + "%";

            // Print the color percentage to the console
            std::cout << text << std::endl;
        }
        for (const auto& tile : blueTiles)
        {
            // Draw bounding box on the original frame
            cv::rectangle(frame, tile.boundingBox, cv::Scalar(255, 0, 0), 2); // Blue color for bounding box

            // Display color percentage on the frame
            std::string text = "Blue Percentage: " + std::to_string(tile.colorPercentage) + "%";

            // Print the color percentage to the console
            std::cout << text << std::endl;
        }

        cv::imshow("Video Input", frame);

        view.alignImages(frame, imReference, imReg, h);

        // imshow kullanarak iki pencereyi açın
        imshow("Aligned Frame", imReg);

        if (waitKey(1) == 27) // Break the loop if Esc is pressed
            break;
    }

    cap.release();
    destroyAllWindows();

    return 0;
}