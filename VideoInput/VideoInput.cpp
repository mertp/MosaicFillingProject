#include <opencv2/opencv.hpp>

int main() {
    cv::VideoCapture cap(0); // Open the default camera (index 0)

    if (!cap.isOpened()) {
        std::cerr << "Error opening the video source." << std::endl;
        return -1;
    }

    cv::namedWindow("Video Input", cv::WINDOW_NORMAL);
    cv::namedWindow("Edge Detection", cv::WINDOW_NORMAL);

    while (true) {
        cv::Mat frame, edgeFrame;
        if (!cap.read(frame)) {
            std::cerr << "Error capturing frame." << std::endl;
            break;
        }

        // Apply Canny edge detection to the frame
        cv::cvtColor(frame, edgeFrame, cv::COLOR_BGR2GRAY); // Convert to grayscale
        cv::Canny(edgeFrame, edgeFrame, 50, 150); // Apply Canny edge detection

        cv::imshow("Video Input", frame);
        cv::imshow("Edge Detection", edgeFrame);

        if (cv::waitKey(1) == 27) { // Exit when the 'Esc' key is pressed
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}
