/*
 * File name    : main.cpp
 * Authors      : Avinash Eega (aeega)
 * Created on   : Mar 25, 2022
 * Credits      : Yasin Yıldırım @ Simit Lab
 * Note         : Majority of the codebase in this file is taken from Yasin
 * Yildirim's git hub. Original source code location :
 * https://github.com/yasinyildirim/CannyEdgeDetector
 */

#include "wilkes/opencv_seq_implementation/CannyEdgeDetector.h"
Mat src;
const char* source_window = " Canny result Image";

int main(int, char** argv) {
    /// Load source image and convert it to gray

    src = imread(argv[1], 1);
    /// Create Window - autosize by default
    // namedWindow(source_window, WINDOW_AUTOSIZE);
    namedWindow(source_window);

    /// Create Trackbar to set the number of corners
    CannyEdgeDetector ced(src);
    ced.detect();

    // result image of step 3
    imshow("Threshold without Hysteresis", ced.thresh);
    // imshow( "Dy", ced.Dy);
    // grayscale of source image
    imshow(" Grayscale", ced.imgGray_copy);
    // imshow( " Grayscale_myversion", ced.imgGray_copy2);

    // canny edge detected image
    imshow(source_window, ced.dest);

    waitKey(0);
    return 0;
}
