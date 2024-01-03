#include "birdview.h"

void birdview::alignImages(cv::Mat& im1, cv::Mat& im2, cv::Mat& im1Reg, cv::Mat& h)
{

    cv::Mat im1Gray, im2Gray;
    cvtColor(im1, im1Gray, cv::COLOR_BGR2GRAY);
    cvtColor(im2, im2Gray, cv::COLOR_BGR2GRAY);

    // Feature extraction
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;
    Ptr<Feature2D> orb = ORB::create(MAX_FEATURES);
    orb->detectAndCompute(im1Gray, Mat(), keypoints1, descriptors1);
    orb->detectAndCompute(im2Gray, Mat(), keypoints2, descriptors2);

    // Match features.
    std::vector<DMatch> matches;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    matcher->match(descriptors1, descriptors2, matches, Mat());

    vector<DMatch> goodMatches;
    for (size_t i = 0; i < matches.size(); i++)
    {
        if (matches[i].distance < DISTANCE_THRESHOLD)
        {
            goodMatches.push_back(matches[i]);
        }
    }

    std::sort(matches.begin(), matches.end());

    const int numGoodMatches = matches.size() * GOOD_MATCH_PERCENT;
    matches.erase(matches.begin() + numGoodMatches, matches.end());
 
   
     Mat imMatches;
     drawMatches(im1, keypoints1, im2, keypoints2, matches, imMatches);
     //imshow("Matches", imMatches);
    
     // Extract location of good matches
    std::vector<Point2f> points1, points2;

    for (size_t i = 0; i < matches.size(); i++)
    {
        points1.push_back(keypoints1[matches[i].queryIdx].pt);
        points2.push_back(keypoints2[matches[i].trainIdx].pt);
    }

    // RANSAC(rastgele �rnek konsens�s�) homografi matrisi
    h = findHomography(points1, points2, RANSAC);

    // Use homography to warp image
    warpPerspective(im1, im1Reg, h, im2.size());
}