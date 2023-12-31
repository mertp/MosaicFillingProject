﻿#include "MosaicFillingProject.h"
#include "birdview.h"
#include "colordetection.h"

cv::Scalar lowerRed1 = cv::Scalar(0, 100, 100);  // Lower bound for red (first range)
cv::Scalar upperRed1 = cv::Scalar(10, 255, 255); // Upper bound for red (first range)

cv::Scalar lowerRed2 = cv::Scalar(160, 100, 100); // Lower bound for red (second range)
cv::Scalar upperRed2 = cv::Scalar(180, 255, 255); // Upper bound for red (second range)

cv::Scalar lowerBlue1 = cv::Scalar(100, 100, 100);
cv::Scalar upperBlue1 = cv::Scalar(120, 255, 255);

int main()
{
    birdview view;
    colordetection detect;

    string refImagePath = "/home/mertp/Desktop/mosaicProject/source/MosaicFillingProject/img4.jpg";
    Mat imReference = imread(refImagePath);
    if (imReference.empty())
    {
        cerr << "Error: Reference image cannot be loaded." << endl;
        return -1;
    }

    VideoCapture cap(0);
    if (!cap.isOpened())
    {
        cerr << "Error: Camera cannot be opened." << endl;
        return -1;
    }

    Mat frame, imReg, h;
    // Define the codec and create VideoWriter objects
    int frameWidth = 540;
    int frameHeight = 764;
    // Video dosyasını kaydetmek için VideoWriter oluşturun
    cv::VideoWriter videoWriter("/Users/Hp/Desktop/MosaicFillingProject/aligned_output.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 5, cv::Size(frameWidth, frameHeight));

    if (!videoWriter.isOpened()) {
        std::cerr << "Error: Video file could not be opened for writing." << std::endl;
        return -1;
    }

    while (true)
    {
        cap >> frame;

        if (frame.empty())
        {
            cerr << "End of video stream." << endl;
            break;
        }

        std::vector<Tile> redTiles, blueTiles;
        double colorPercentage;
        detect.colorDetection(frame, lowerRed1, upperRed1, redTiles);
        detect.colorDetection(frame, lowerBlue1, upperBlue1, blueTiles);
        for (const auto &tile : redTiles)
        {
            // Draw bounding box on the original frame
            cv::rectangle(frame, tile.boundingBox, cv::Scalar(0, 0, 255), 2); // Red color for bounding box

            // Display color percentage on the frame
            std::string text = "Red Tile Area: " + std::to_string(tile.area);

            cv::putText(frame, text, cv::Point(tile.boundingBox.x + tile.boundingBox.width + 10, tile.boundingBox.y),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);

            // Print the color percentage to the console
            // std::cout << text << std::endl;
        }
        for (const auto &tile : blueTiles)
        {
            // Draw bounding box on the original frame
            cv::rectangle(frame, tile.boundingBox, cv::Scalar(255, 0, 0), 2); // Blue color for bounding box

            // Display color percentage on the frame
            std::string text = "Blue Area " + std::to_string(tile.area);

            cv::putText(frame, text, cv::Point(tile.boundingBox.x + tile.boundingBox.width + 10, tile.boundingBox.y),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);

            // Print the color percentage to the console
            // std::cout << text << std::endl;
        }

        // Write the frame to the mosaic video output
        // mosaicVideo.write(frame);
        std::cout << "Writing frame to mosaic video." << std::endl;

        cv::imshow("Video Input", frame);

        view.alignImages(frame, imReference, imReg, h);

        // imReg'in boyutlarını kontrol et ve frameWidth ve frameHeight ile uyumlu hale getir
        cv::resize(imReg, imReg, cv::Size(frameWidth, frameHeight));

        cv::imshow("Aligned Frame", imReg);
        // Video dosyasına yazma
        videoWriter.write(imReg);

        if (waitKey(1) == 27)
        {

            // Break the loop if Esc is pressed
            break;
        }
    }


    videoWriter.release();
    cap.release();

    destroyAllWindows();

    return 0;
}