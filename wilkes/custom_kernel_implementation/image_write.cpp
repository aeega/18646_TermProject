#include <opencv2/ml.hpp>
#include <opencv2/opencv.hpp>

#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    /* for (int i = 0 ; i < image2.rows; i++) {
         for (int j = 0 ; j < image2.cols; j++) {
             cout << "Pixel intensity " << image2.at<uchar>(i,j) << endl;
         }
     }*/
    cv::Mat image1, image2, final_img;
    image1 = cv::imread(argv[1]);
    cv::cvtColor(image1, image2, cv::COLOR_BGRA2GRAY);

    cv::Ptr<cv::ml::TrainData> mldata, mldatax, mldatay, mldatafinal;
    mldata = cv::ml::TrainData::loadFromCSV("output.csv", 0);
    mldatax = cv::ml::TrainData::loadFromCSV("outputx.csv", 0);
    mldatay = cv::ml::TrainData::loadFromCSV("outputy.csv", 0);
    mldatafinal = cv::ml::TrainData::loadFromCSV("output_dnew.csv", 0);

    cv::Mat img = mldata->getSamples();
    cv::Mat imgx = mldatax->getSamples();
    cv::Mat imgy = mldatay->getSamples();
    cv::Mat imgfinal = mldatafinal->getSamples();

    img.convertTo(img, CV_8UC3);
    imgx.convertTo(imgx, CV_8UC3);
    imgy.convertTo(imgy, CV_8UC3);
    imgfinal.convertTo(imgfinal, CV_8UC3);

    img = img.reshape(1);
    imgx = imgx.reshape(1);
    imgy = imgy.reshape(1);
    imgfinal = imgfinal.reshape(1);

    // resize(img, final_img , Size(image2.cols - 2, image2.rows - 2),
    // INTER_LINEAR);
    cv::imshow("Orig", image2);
    // waitKey(0);
    // destroyWindow("Orig");
    cv::imshow("img", img);
    cv::imshow("imgx", imgx);
    cv::imshow("imgy", imgy);
    // imshow("imgfinal", imgfinal);
    cv::waitKey(0);
    cv::destroyWindow("img");
    /*//imwrite(filename, image2);
    //ReadMatFromTxt(filename, 480, 737);
    std::ifstream file("test.txt");
    Mat image1;
    int rows = 0;
    char space = ", ";
    std::string line;
    while (std::my_getline(file, line)) {

        int columns = 0;
        std::istringstream stream(line);
        int x;

        while(stream >> x && stream >> space) {
            image1.push_back(x);
            columns++;

        }
        cout << "Columns " << columns << " " ;
        rows++;
    }
    cout << "Rows : " << rows << endl;
    image1 = image1.reshape(1, rows);
    //imshow("Flower", image1);
    //waitKey(0);
    //destroyWindow("Flower");*/
}
