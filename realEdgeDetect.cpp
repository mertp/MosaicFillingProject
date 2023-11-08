#include <opencv2/opencv.hpp>

int main() 
{
    // Video capture object to capture frames from the camera
    cv::VideoCapture cap(0); // 0 means the default camera, you can change it to the camera index you want to use

    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    cv::Mat frame, edges;
    cv::namedWindow("Real-time Edge Detection", cv::WINDOW_NORMAL);
    cv::resizeWindow("Real-time Edge Detection", 640, 480);

    while (true) {
        // Capture frame from the camera
        cap >> frame;

        // Convert the frame to grayscale
        cv::cvtColor(frame, edges, cv::COLOR_BGR2GRAY);

        // Apply Canny edge detection
        cv::Canny(edges, edges, 100, 200);

        // Display the resulting frame with edges
        cv::imshow("Real-time Edge Detection", edges);

        // Break the loop if 'q' is pressed
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    // Release the video capture object and close the windows
    cap.release();
    cv::destroyAllWindows();

    return 0;
}
