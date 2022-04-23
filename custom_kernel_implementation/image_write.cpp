#include <opencv2/opencv.hpp>
#include <iostream>
#include<fstream>

using namespace std;
using namespace cv;

/*Mat ReadMatFromTxt(string filename, int rows,int cols)
{
    double m;
    Mat out = Mat::zeros(rows, cols, CV_64FC1);//Matrix to store values

    ifstream fileStream(filename);
    int cnt = 0;//index starts from 0
    while (fileStream >> m)
    {
        int temprow = cnt / cols;
        int tempcol = cnt % cols;
        out.at<double>(temprow, tempcol) = m;
        cnt++;
    }
    return out;
}*/

int main(int argc, char* argv[]) {
     
   /* for (int i = 0 ; i < image2.rows; i++) {
        for (int j = 0 ; j < image2.cols; j++) {
            cout << "Pixel intensity " << image2.at<uchar>(i,j) << endl;
        }
    }*/
    Mat image1, image2, final_img;
    image1 = imread(argv[1]);
    cvtColor(image1,image2, CV_BGR2GRAY);

    CvMLData mldata, mldatax, mldatay;
    mldata.read_csv("output.csv");
    mldatax.read_csv("outputx.csv");
    mldatay.read_csv("outputy.csv");
    const CvMat* tmp = mldata.get_values();
    const CvMat* tmpx = mldatax.get_values();
    const CvMat* tmpy = mldatay.get_values();
    Mat img(tmp, true);
    Mat imgx(tmpx, true);
    Mat imgy(tmpy, true);

    img.convertTo(img, CV_8UC3);
    imgx.convertTo(imgx, CV_8UC3);
    imgy.convertTo(imgy, CV_8UC3);
    img = img.reshape(1);
    imgx = imgx.reshape(1);
    imgy = imgy.reshape(1);

    //resize(img, final_img , Size(image2.cols - 2, image2.rows - 2), INTER_LINEAR);
    imshow("Orig", image2);
    //waitKey(0);
    //destroyWindow("Orig");
    imshow("img", img);
    imshow("imgx", imgx);
    imshow("imgy", imgy);
    waitKey(0);
    destroyWindow("img");
    /*//imwrite(filename, image2);
    //ReadMatFromTxt(filename, 480, 737);
    std::ifstream file("test.txt");
    Mat image1;
    int rows = 0;
    char space = ", ";
    std::string line;
    while (std::getline(file, line)) {

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
