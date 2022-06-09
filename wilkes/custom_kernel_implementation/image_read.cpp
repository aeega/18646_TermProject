#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char* argv[]) {
    cv::Mat image1, image2, image3;
    image1 = cv::imread(argv[1]);
    cv::cvtColor(image1, image2, cv::COLOR_BGRA2GRAY);
    //    namedWindow("Flower", WINDOW_AUTOSIZE);
    //    imshow("Flower", image2);
    //    waitKey(0);
    //    destroyWindow("Flower");

    /*for (int i = 0 ; i < image2.rows; i++) {
        for (int j = 0 ; j < image2.cols; j++) {
            if (image2.at<uchar>(i,j) >= 127) {
                image3.at<uchar>(i,j) = 254;
            } else {
                image3.at<uchar>(i,j) = 0;
            }
            cout << "Pixel intensity " << image3.at<uchar>(i,j) << endl;
        }
    }*/

    cout << "Num rows " << image2.rows << "    Num columns " << image2.cols
         << endl;
    //    imshow("Threshold", image3);
    const char* filename = "test.csv";
    ofstream myfile;
    myfile.open(filename);
    myfile << cv::format(image2, cv::Formatter::FMT_CSV) << endl;
    myfile.close();

    // imwrite(filename, image2);
    // writetofile(image2, filename);
}
