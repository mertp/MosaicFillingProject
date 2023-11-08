#include <opencv2/opencv.hpp>

int main() {
    cv::VideoCapture cap(-1); // Open the default camera (index 0)

    if (!cap.isOpened()) {
        std::cerr << "Error opening the video source." << std::endl;
        return -1;
    }

    cv::namedWindow("Video Input", cv::WINDOW_NORMAL);

    while (true) {
        cv::Mat frame;
        if (!cap.read(frame)) {
            std::cerr << "Error capturing frame." << std::endl;
            break;
        }

        cv::imshow("Video Input", frame);

        if (cv::waitKey(50) == 27) { // Exit when the 'Esc' key is pressed
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}
