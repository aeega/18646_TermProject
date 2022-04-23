#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

void writetofile (cv::Mat& m, const char* filename) {
    ofstream fout(filename);

    if (!fout) {
        cout << "File not found" << endl;
        return;
    }
    //fout << m.rows << " " << m.cols << " ";
    for (int i = 0 ; i < m.rows; i++) {
        for (int j = 0 ; j < m.cols; j++) {
 //           uchar pixel_intensity = m.at<uchar>(i,j);
 //           cout << "Pixel intensity " << pixel_intensity << endl;
            fout << m.at<float>(i,j) << " ";
        }
       // fout << endl;
    }
    fout.close();
}

using namespace cv;
int main(int argc, char *argv[]) {

    Mat image1, image2, image3;
    image1 = imread(argv[1]);
    cvtColor(image1,image2, CV_BGR2GRAY);
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

    cout << "Num rows " << image2.rows << "    Num columns " << image2.cols << endl; 
//    imshow("Threshold", image3);
    const char* filename = "test.csv";
    ofstream myfile;
    myfile.open(filename);
    myfile << format(image2, "csv") << endl;
    myfile.close();

    //imwrite(filename, image2);
    //writetofile(image2, filename);
}
