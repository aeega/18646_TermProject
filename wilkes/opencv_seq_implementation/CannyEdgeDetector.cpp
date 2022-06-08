/*
 * File name    : CannyEdgeDetector.cpp
 * Authors      : Avinash Eega (aeega)
 * Created on   : Mar 25, 2022
 * Credits      : Yasin Yıldırım @ Simit Lab
 * Note         : Majority of the codebase in this file is taken from Yasin
 * Yildirim's git hub. orignal code :
 * https://github.com/yasinyildirim/CannyEdgeDetector Updates      : 1. The
 * original code base is implemented with 5x5 kernels in every stage. We have
 * updated the code base to support a 3x3 kernel implementation.
 *                2. Included rdtsc() functions to measure time taken for each
 * stage to execute
 *                3. Updated the input& output datatypes at each stage to
 * <float> instead of <uchar> so the performance comparison is accurate.
 *                4. Display images after each stage instead of a single final
 * output after processing all CED stages
 */

#include "wilkes/opencv_seq_implementation/CannyEdgeDetector.h"

#include <immintrin.h>
#ifdef _WIN32
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

// the __rdtsc intrinsic is defined automatically by either intrin.h or
// x86intrin.h unsigned long long rdtsc();
#define rdtsc __rdtsc

unsigned long long total_time = 0;
unsigned long long total_time2 = 0;
unsigned long long total_time3 = 0;
unsigned long long total_time4 = 0;
unsigned long long total_time5 = 0;
int counter = 0;

CannyEdgeDetector::CannyEdgeDetector() {
    // TODO Auto-generated constructor stub
}

CannyEdgeDetector::CannyEdgeDetector(Mat src) {
    // Load original image and store it as a grayscale image

    imgGray = Mat::zeros(src.rows, src.cols, CV_8UC1);
    dest = Mat::zeros(src.rows, src.cols, CV_8UC1);
    Dx = Mat::zeros(src.rows, src.cols, CV_16SC1);
    Dy = Mat::zeros(src.rows, src.cols, CV_16SC1);
    D = Mat::zeros(src.rows, src.cols, CV_32FC1);
    Teta = Mat::zeros(src.rows, src.cols, CV_32FC1);
    thresh = Mat::zeros(src.rows, src.cols, CV_8UC1);
    D_new = Mat::zeros(src.rows, src.cols, CV_32FC1);
    imgGray_copy;
    // imgGray_copy2;

    const int channels = src.channels();

    // on the fly matrix element access
    switch (channels) {
    case 1: {
        for (int i = 0; i < src.rows; ++i) {
            for (int j = 0; j < src.cols; ++j) {
                imgGray.at<uchar>(i, j) = src.at<uchar>(i, j);
            }
        }

        break;
    }
    case 3: {
        Mat_<Vec3b> _I = src;

        for (int i = 0; i < src.rows; ++i)
            for (int j = 0; j < src.cols; ++j) {
                imgGray.at<uchar>(i, j) =
                  (uchar)(_I(i, j)[0] * 0.114 + _I(i, j)[1] * 0.587 +
                          _I(i, j)[2] * 0.299);
            }
        src = _I;
    }
    }
    imgGray_copy = imgGray.clone();
}

CannyEdgeDetector::~CannyEdgeDetector() {
    // TODO Auto-generated destructor stub
}

void CannyEdgeDetector::detect() {
    counter++;

    const int BLOCK_SIZE = 3;
    const int BLOCK_SIZE3X = 3;
    int rowBlocks = imgGray.rows - BLOCK_SIZE;
    int colBlocks = imgGray.cols - BLOCK_SIZE;

    int rowBlocks3x = imgGray.rows - BLOCK_SIZE3X;
    int colBlocks3x = imgGray.cols - BLOCK_SIZE3X;
    // noise reduction
    float GaussianKernel[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
    int DxKernel[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int DyKernel[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    printf("ImgGray.rows = %d\n", imgGray.rows);
    printf("ImgGray.cols = %d\n", imgGray.cols);
    unsigned long long et = 0, st = 0;
    unsigned long long et2 = 0, st2 = 0;
    unsigned long long et3 = 0, st3 = 0;
    unsigned long long et4 = 0, st4 = 0;
    unsigned long long et5 = 0, st5 = 0;
    st = rdtsc();

    // Step 1: Noise Reduction
    for (int i = 0; i < rowBlocks; i++) {
        for (int j = 0; j < colBlocks; j++) {
            Mat windowImage_ij(imgGray, Rect(j, i, BLOCK_SIZE, BLOCK_SIZE));
            // block processing loops
            float gbValues[3][3] = {
              {0}};  // Gaussian blur resulting float values
            vector<float> vgbValues;
            for (int k = 0; k < windowImage_ij.rows; k++) {
                for (int m = 0; m < windowImage_ij.cols; m++) {
                    gbValues[k][m] =
                      windowImage_ij.at<uchar>(k, m) * GaussianKernel[k][m];
                    vgbValues.push_back(
                      (windowImage_ij.at<uchar>(k, m) * GaussianKernel[k][m]));
                }
            }
            for (int k = 0; k < windowImage_ij.rows; k++) {
                for (int m = 0; m < windowImage_ij.cols; m++) {
                    if (k == 1 && m == 1) continue;
                    gbValues[1][1] += gbValues[k][m];
                    vgbValues[4] += gbValues[k][m];
                }
            }

            imgGray.at<uchar>(i + 1, j + 1) = (uchar)(vgbValues[4] / 16);
            vgbValues.clear();
        }
    }
    // imgGray_copy2 = imgGray.clone();
    et = rdtsc() - st;
    total_time = total_time + et;
    // DBG_PRINTS
    // printf("Number of iterations = %d\n; Total time taken = %f\n avg exec
    // time per iteration = %f\n", counter, (float)total_time,
    // (float)(total_time/counter));

    // Step 2: Compute Gradient Magnitude and Angle
    for (int i = 0; i < rowBlocks3x; i++) {
        st2 = rdtsc();
        for (int j = 0; j < colBlocks3x; j++) {
            Mat windowImagex_ij(imgGray,
                                Rect(j, i, BLOCK_SIZE3X, BLOCK_SIZE3X));
            Mat windowImagey_ij(imgGray,
                                Rect(j, i, BLOCK_SIZE3X, BLOCK_SIZE3X));
            // block processing loops
            for (int k = 0; k < windowImagex_ij.rows; k++) {
                for (int m = 0; m < windowImagex_ij.cols; m++) {
                    windowImagex_ij.at<int>(k, m) =
                      windowImagex_ij.at<int>(k, m) * DxKernel[k][m];
                    windowImagey_ij.at<int>(k, m) =
                      windowImagey_ij.at<int>(k, m) * DyKernel[k][m];
                }
            }

            for (int k = 0; k < windowImagex_ij.rows; k++) {
                for (int m = 0; m < windowImagex_ij.cols; m++) {
                    if (k == 1 && m == 1) continue;
                    windowImagex_ij.at<int>(1, 1) +=
                      windowImagex_ij.at<int>(k, m);
                    windowImagey_ij.at<int>(1, 1) +=
                      windowImagey_ij.at<int>(k, m);
                }
            }

            Dx.at<int>(i + 1, j + 1) = windowImagex_ij.at<int>(1, 1);
            Dy.at<int>(i + 1, j + 1) = windowImagey_ij.at<int>(1, 1);
            D.at<float>(i + 1, j + 1) =
              hypotf(static_cast<float>(Dx.at<int>(i + 1, j + 1)),
                     static_cast<float>(Dy.at<int>(i + 1, j + 1)));
            if (Dy.at<int>(i + 1, j + 1) == 0) {
                Teta.at<float>(i + 1, j + 1) = 90;
            } else {
                Teta.at<float>(i + 1, j + 1) =
                  atan2f(static_cast<float>(Dy.at<int>(i + 1, j + 1)),
                         static_cast<float>(Dx.at<int>(i + 1, j + 1)));
            }
            if (Teta.at<float>(i + 1, j + 1) > 180) {
                Teta.at<float>(i + 1, j + 1) -= 180;
            } else if (Teta.at<float>(i + 1, j + 1) < 0) {
                Teta.at<float>(i + 1, j + 1) += 180;
            }

            if (Teta.at<float>(i + 1, j + 1) <= 22.5 ||
                Teta.at<float>(i + 1, j + 1) >= 157.5) {
                Teta.at<float>(i + 1, j + 1) = 0;
            } else if (Teta.at<float>(i + 1, j + 1) > 22.5 &&
                       Teta.at<float>(i + 1, j + 1) <= 67.5) {
                Teta.at<float>(i + 1, j + 1) = 45;
            } else if (Teta.at<float>(i + 1, j + 1) > 67.5 &&
                       Teta.at<float>(i + 1, j + 1) <= 112.5) {
                Teta.at<float>(i + 1, j + 1) = 90;
            } else if (Teta.at<float>(i + 1, j + 1) > 112.5 &&
                       Teta.at<float>(i + 1, j + 1) < 157.5) {
                Teta.at<float>(i + 1, j + 1) = 135;
            }
        }
    }
    et2 = rdtsc() - st2;
    total_time2 = total_time2 + et2;

    // DBG_PRINTS
    // printf("Total time = %f\n avg exec time = %f\n", (float)total_time2,
    // (float)total_time2/counter); printf("Number of iterations = %d\n; Total
    // time taken = %f\n avg exec time per iteration = %f\n", counter,
    // (float)total_time2, (float)(total_time2/counter));

    printf("Total cycles taken for two stages  = %f\n",
           ((float)total_time + (float)total_time2) / counter);
    // Step 3: Non-Maximum Surpression
    st3 = rdtsc();
    for (int i = 0; i < rowBlocks3x; i++) {
        for (int j = 0; j < colBlocks3x; j++) {
            Mat windowImage_ij(D, Rect(j, i, BLOCK_SIZE3X, BLOCK_SIZE3X));
            Mat windowImageTeta_ij(Teta,
                                   Rect(j, i, BLOCK_SIZE3X, BLOCK_SIZE3X));
            // block processing loops

            if (windowImageTeta_ij.at<float>(1, 1) == 0) {
                if (windowImage_ij.at<float>(1, 1) >
                      windowImage_ij.at<float>(2, 1) &&
                    windowImage_ij.at<float>(1, 1) >
                      windowImage_ij.at<float>(0, 1)) {
                    thresh.at<uchar>(i + 1, j + 1) = 255;
                    D_new.at<float>(i + 1, j + 1) =
                      windowImage_ij.at<float>(1, 1);
                }

            } else if (windowImageTeta_ij.at<float>(1, 1) == 45) {
                if (windowImage_ij.at<float>(1, 1) >
                      windowImage_ij.at<float>(2, 2) &&
                    windowImage_ij.at<float>(1, 1) >
                      windowImage_ij.at<float>(0, 0)) {
                    thresh.at<uchar>(i + 1, j + 1) = 255;
                    D_new.at<float>(i + 1, j + 1) =
                      windowImage_ij.at<float>(1, 1);
                }
            } else if (windowImageTeta_ij.at<float>(1, 1) == 90) {
                if (windowImage_ij.at<float>(1, 1) >
                      windowImage_ij.at<float>(1, 2) &&
                    windowImage_ij.at<float>(1, 1) >
                      windowImage_ij.at<float>(1, 0)) {
                    thresh.at<uchar>(i + 1, j + 1) = 255;
                    D_new.at<float>(i + 1, j + 1) =
                      windowImage_ij.at<float>(1, 1);
                }
            } else if (windowImageTeta_ij.at<float>(1, 1) == 135) {
                if (windowImage_ij.at<float>(1, 1) >
                      windowImage_ij.at<float>(2, 0) &&
                    windowImage_ij.at<float>(1, 1) >
                      windowImage_ij.at<float>(0, 2)) {
                    thresh.at<uchar>(i + 1, j + 1) = 255;
                    D_new.at<float>(i + 1, j + 1) =
                      windowImage_ij.at<float>(1, 1);
                }
            }
        }
    }
    et3 = rdtsc() - st3;
    total_time3 = total_time3 + et3;

    st4 = rdtsc();
    // Step 4: Hysteresis Thresholding
    Scalar meanValue = mean(D, thresh);
    cout << "The meanValue is " << meanValue[0] << endl;
    float thigh = static_cast<float>(meanValue[0]);
    float tlow = thigh / 2;

    for (int i = 0; i < rowBlocks3x; i++) {
        for (int j = 0; j < colBlocks3x; j++) {
            Mat windowImage_ij(D_new, Rect(j, i, BLOCK_SIZE3X, BLOCK_SIZE3X));
            Mat windowImage5x_ij;
            if (rowBlocks3x + BLOCK_SIZE < imgGray.rows &&
                colBlocks3x + BLOCK_SIZE < imgGray.cols) {
                Mat windowImage5x_ij(D_new, Rect(j, i, BLOCK_SIZE, BLOCK_SIZE));
            }

            // block processing loops
            bool hasHighValue = false;
            bool hasAtLeastOneAverageValue = false;
            if (windowImage_ij.at<float>(1, 1) > thigh) {
                dest.at<uchar>(i + 1, j + 1) = 255;

            } else if (windowImage_ij.at<float>(1, 1) < tlow) {
                continue;
            } else {
                for (int k = 0; k < windowImage_ij.rows; k++) {
                    for (int m = 0; m < windowImage_ij.cols; m++) {
                        if (k == 1 && m == 1) { continue; }
                        if (windowImage_ij.at<float>(k, m) > thigh) {
                            hasHighValue = true;
                            break;
                        } else if (windowImage_ij.at<float>(k, m) > tlow) {
                            hasAtLeastOneAverageValue = true;
                        }
                    }
                }
                if (hasHighValue) {
                    dest.at<uchar>(i + 1, j + 1) = 255;
                } else if (hasAtLeastOneAverageValue &&
                           rowBlocks3x + BLOCK_SIZE < imgGray.rows &&
                           colBlocks3x + BLOCK_SIZE < imgGray.cols) {
                    for (int k = 0; k < windowImage5x_ij.rows; k++) {
                        for (int m = 0; m < windowImage5x_ij.cols; m++) {
                            if (k == 2 && m == 2) { continue; }

                            if (windowImage5x_ij.at<float>(k, m) > thigh) {
                                hasHighValue = true;
                                break;
                            }
                        }
                    }

                    if (hasHighValue) { dest.at<uchar>(i + 1, j + 1) = 255; }
                }
            }
        }
    }
    et4 = rdtsc() - st4;
    total_time4 = total_time4 + et4;
    printf("[SEQ]:Time for stage 1  = %lld\n", et / counter);
    printf("[SEQ]:Time for stage 2  = %lld\n", et2);
    printf("[SEQ]:Time for stage 3  = %lld\n", et3);
    printf("[SEQ]:Time for stage4&5 = %lld\n", et4);
    printf("[SEQ]:Total cycles taken for two stages  = %lld\n",
           (total_time + total_time2) / counter);
    printf("[SEQ]:Total cycles taken for all stages sequential = %lld\n",
           (total_time + total_time2 + total_time3 + total_time4) / counter);
}

// With low and high thresholds
// void CannyEdgeDetector::detect(int lowThreshold, int highThreshold) {
//
//	const int BLOCK_SIZE = 3;
//	const int BLOCK_SIZE3X = 3;
//	int rowBlocks = imgGray.rows - BLOCK_SIZE;
//	int colBlocks = imgGray.cols - BLOCK_SIZE;
//
//	int rowBlocks3x = imgGray.rows - BLOCK_SIZE3X;
//	int colBlocks3x = imgGray.cols - BLOCK_SIZE3X;
//	// Noise reduction
//	float GaussianKernel[3][3] = {
//			{ 1 / 16, 2 / 16, 1 / 16 }, { 2 / 16, 4 / 16, 2 / 16 }, { 1 / 16, 2
/// 16, 1 / 16 } }; 	int DxKernel[3][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1,
// 0, 1 } }; 	int DyKernel[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 }
//};
//
//	// Step 1: Noise Reduction
//	for (int i = 0; i < rowBlocks; i++) {
//		for (int j = 0; j < colBlocks; j++) {
//			Mat windowImage_ij(imgGray, Rect(j, i, BLOCK_SIZE, BLOCK_SIZE));
//			// block processing loops
//			float gbValues[3][3] = { { 0 } }; // Gaussian blur resulting float
// values 			for (int k = 0; k < windowImage_ij.rows; k++) {
//
//				for (int m = 0; m < windowImage_ij.cols; m++) {
//					gbValues[k][m] = windowImage_ij.at<uchar>(k, m)
//							* GaussianKernel[k][m];
//				}
//			}
//			for (int k = 0; k < windowImage_ij.rows; k++) {
//
//				for (int m = 0; m < windowImage_ij.cols; m++) {
//					if (k == 1 && m == 1)
//						continue;
//					gbValues[1][1] += gbValues[k][m];
//				}
//			}
//
//			//imgGray.at<uchar>(i + 2, j + 2) = (uchar) gbValues[2][2];
//
//		}
//	}
//
//	//Step 2: Compute Gradient Magnitude and Angle
//	for (int i = 0; i < rowBlocks3x; i++) {
//		for (int j = 0; j < colBlocks3x; j++) {
//			Mat windowImagex_ij(imgGray, Rect(j, i, BLOCK_SIZE3X,
// BLOCK_SIZE3X)); 			Mat windowImagey_ij(imgGray, Rect(j, i,
// BLOCK_SIZE3X, BLOCK_SIZE3X));
//			// block processing loops
//			for (int k = 0; k < windowImagex_ij.rows; k++) {
//
//				for (int m = 0; m < windowImagex_ij.cols; m++) {
//					windowImagex_ij.at<int>(k, m) = windowImagex_ij.at<int>(k,
//							m) * DxKernel[k][m];
//					windowImagey_ij.at<int>(k, m) = windowImagey_ij.at<int>(k,
//							m) * DyKernel[k][m];
//				}
//			}
//
//			for (int k = 0; k < windowImagex_ij.rows; k++) {
//
//				for (int m = 0; m < windowImagex_ij.cols; m++) {
//					if (k == 1 && m == 1)
//						continue;
//					windowImagex_ij.at<int>(1, 1) += windowImagex_ij.at<int>(k,
//							m);
//					windowImagey_ij.at<int>(1, 1) += windowImagey_ij.at<int>(k,
//							m);
//				}
//			}
//
//			Dx.at<int>(i + 1, j + 1) = windowImagex_ij.at<int>(1, 1);
//			Dy.at<int>(i + 1, j + 1) = windowImagey_ij.at<int>(1, 1);
//			D.at<float>(i + 1, j + 1) = hypot(Dx.at<int>(i + 1, j + 1),
//					Dy.at<int>(i + 1, j + 1));
//			if (Dy.at<int>(i + 1, j + 1) == 0) {
//				Teta.at<float>(i + 1, j + 1) = 90;
//			} else {
//				Teta.at<float>(i + 1, j + 1) = atan(
//						(Dy.at<int>(i + 1, j + 1) / Dx.at<int>(i + 1, j + 1)));
//			}
//			if (Teta.at<float>(i + 1, j + 1) > 180) {
//				Teta.at<float>(i + 1, j + 1) -= 180;
//			} else if (Teta.at<float>(i + 1, j + 1) < 0) {
//				Teta.at<float>(i + 1, j + 1) += 180;
//			}
//
//			if (Teta.at<float>(i + 1, j + 1) <= 22.5
//					|| Teta.at<float>(i + 1, j + 1) >= 157.5) {
//				Teta.at<float>(i + 1, j + 1) = 0;
//			} else if (Teta.at<float>(i + 1, j + 1) > 22.5
//					&& Teta.at<float>(i + 1, j + 1) <= 67.5) {
//				Teta.at<float>(i + 1, j + 1) = 45;
//			} else if (Teta.at<float>(i + 1, j + 1) > 67.5
//					&& Teta.at<float>(i + 1, j + 1) <= 112.5) {
//				Teta.at<float>(i + 1, j + 1) = 90;
//			} else if (Teta.at<float>(i + 1, j + 1) > 112.5
//					&& Teta.at<float>(i + 1, j + 1) < 157.5) {
//				Teta.at<float>(i + 1, j + 1) = 135;
//			}
//		}
//	}
//
//	//Step 3: Non-Maximum Surpression
//
//	for (int i = 0; i < rowBlocks3x; i++) {
//		for (int j = 0; j < colBlocks3x; j++) {
//			Mat windowImage_ij(D, Rect(j, i, BLOCK_SIZE3X, BLOCK_SIZE3X));
//			Mat windowImageTeta_ij(Teta,
//					Rect(j, i, BLOCK_SIZE3X, BLOCK_SIZE3X));
//			// block processing loops
//
//			if (windowImageTeta_ij.at<float>(1, 1) == 0) {
//				if (windowImage_ij.at<float>(1, 1)
//						> windowImage_ij.at<float>(2, 1)
//						&& windowImage_ij.at<float>(1, 1)
//								> windowImage_ij.at<float>(0, 1)) {
//					thresh.at<uchar>(i + 1, j + 1) = 255;
//					D_new.at<float>(i + 1, j + 1) = windowImage_ij.at<float>(1,
//							1);
//				}
//
//			} else if (windowImageTeta_ij.at<float>(1, 1) == 45) {
//				if (windowImage_ij.at<float>(1, 1)
//						> windowImage_ij.at<float>(2, 2)
//						&& windowImage_ij.at<float>(1, 1)
//								> windowImage_ij.at<float>(0, 0)) {
//					thresh.at<uchar>(i + 1, j + 1) = 255;
//					D_new.at<float>(i + 1, j + 1) = windowImage_ij.at<float>(1,
//							1);
//				}
//			} else if (windowImageTeta_ij.at<float>(1, 1) == 90) {
//				if (windowImage_ij.at<float>(1, 1)
//						> windowImage_ij.at<float>(1, 2)
//						&& windowImage_ij.at<float>(1, 1)
//								> windowImage_ij.at<float>(1, 0)) {
//					thresh.at<uchar>(i + 1, j + 1) = 255;
//					D_new.at<float>(i + 1, j + 1) = windowImage_ij.at<float>(1,
//							1);
//				}
//			} else if (windowImageTeta_ij.at<float>(1, 1) == 135) {
//				if (windowImage_ij.at<float>(1, 1)
//						> windowImage_ij.at<float>(2, 0)
//						&& windowImage_ij.at<float>(1, 1)
//								> windowImage_ij.at<float>(0, 2)) {
//					thresh.at<uchar>(i + 1, j + 1) = 255;
//					D_new.at<float>(i + 1, j + 1) = windowImage_ij.at<float>(1,
//							1);
//				}
//			}
//
//		}
//	}
//
//	//Step 4: Hysteresis Thresholding
//	Scalar meanValue = mean(D, thresh);
//	float thigh = meanValue[0];
//	float tlow = thigh / 2;
//
//	for (int i = 0; i < rowBlocks3x; i++) {
//		for (int j = 0; j < colBlocks3x; j++) {
//			Mat windowImage_ij(D_new, Rect(j, i, BLOCK_SIZE3X, BLOCK_SIZE3X));
//			Mat windowImage5x_ij;
//			if (rowBlocks3x + BLOCK_SIZE < imgGray.rows
//					&& colBlocks3x + BLOCK_SIZE < imgGray.cols) {
//				Mat windowImage5x_ij(D_new, Rect(j, i, BLOCK_SIZE, BLOCK_SIZE));
//			}
//
//			// block processing loops
//			bool hasHighValue = false;
//			bool hasAtLeastOneAverageValue = false;
//			if (windowImage_ij.at<float>(1, 1) > thigh) {
//				dest.at<uchar>(i + 1, j + 1) = 255;
//
//			} else if (windowImage_ij.at<float>(1, 1) < tlow) {
//				continue;
//			} else {
//				for (int k = 0; k < windowImage_ij.rows; k++) {
//					for (int m = 0; m < windowImage_ij.cols; m++) {
//						if (k == 1 && m == 1) {
//							continue;
//						}
//						if (windowImage_ij.at<float>(k, m) > thigh) {
//							hasHighValue = true;
//							break;
//						} else if (windowImage_ij.at<float>(k, m) > tlow) {
//							hasAtLeastOneAverageValue = true;
//						}
//					}
//				}
//				if (hasHighValue) {
//					dest.at<uchar>(i + 1, j + 1) = 255;
//				} else if (hasAtLeastOneAverageValue
//						&& rowBlocks3x + BLOCK_SIZE < imgGray.rows
//						&& colBlocks3x + BLOCK_SIZE < imgGray.cols) {
//					for (int k = 0; k < windowImage5x_ij.rows; k++) {
//						for (int m = 0; m < windowImage5x_ij.cols; m++) {
//							if (k == 1 && m == 1) {
//								continue;
//							}
//
//							if (windowImage5x_ij.at<float>(k, m) > thigh) {
//								hasHighValue = true;
//								break;
//							}
//
//						}
//					}
//
//					if (hasHighValue) {
//						dest.at<uchar>(i + 1, j + 1) = 255;
//					}
//
//				}
//			}
//
//		}
//	}
//
//}
//
//
//
