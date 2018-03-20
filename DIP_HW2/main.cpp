//
//  main.cpp
//  DIP_HW2
//
//  Created by 王瀚磊 on 2016/10/19.
//  Copyright © 2016年 王瀚磊. All rights reserved.
//

#include <iostream>
#include <opencv.hpp>

using namespace cv;
using namespace std;

void drawHistogram(Mat src, String type);
void gamma_correct(Mat src,  Mat &dst, float fGamma, float fDegradation = 1);
int computeOutput(int, int, int, int, int );
void full_scale_stretch(Mat src);
void hist_equalize(Mat src);
void unsharping_mask(Mat src, bool unsharp=false, float kMask = 1);


//    ./DIP_HWX source_path dest_path

String dest_path;
//*******************************************************************************//
int main(int argc, const char * argv[]) {
    Mat src, dst;
    dest_path = argv[2];
    src = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
    
    if(!src.data){
        cout<< "Source image file not found !!\nGoing to shut down."<<endl;
        return -1;
    }
    
    drawHistogram(src, "source");
    gamma_correct(src, dst, 2.5);
    gamma_correct(src, dst, 2.5, 0.6);
    full_scale_stretch(dst);
    hist_equalize(dst);
    unsharping_mask(src, true, 3);

    return 0;
}
//*******************************************************************************//



//draw hsitogram
void drawHistogram(Mat src, String type){
    Mat output_hist;
    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };

    calcHist(&src, 1, 0, Mat(), output_hist, 1, &histSize, &histRange, true, true);
    
    int hist_w = 256; int hist_h = 300;
    int bin_w = cvRound( (double) hist_w/histSize );
    
    Mat histImage( hist_h, hist_w, CV_8UC1, Scalar( 0,0,0) );
    normalize(output_hist, output_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

    
    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
        line(histImage, Point( bin_w*(i-1), hist_h - cvRound(output_hist.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(output_hist.at<float>(i)) ),
             Scalar( 255, 255 , 255));
        
    }
    imwrite(dest_path + type+ "_histogram.jpg", histImage);
}

//homework2..gamma correction and reduce dynamic range
void gamma_correct(Mat src,  Mat &dst, float fGamma, float fDegradation){
    src.convertTo(src, CV_32FC1);
    dst = Mat::zeros(src.rows, src.cols, CV_32FC1);
    for(int i =0; i<src.rows; i++){
        for(int j = 0; j<src.cols; j++){
            dst.at<float>(i,j) = pow((float)(src.at<float>(i,j)/255.0), fGamma) * 255.0* fDegradation;
        }
    }
    
    dst.convertTo(dst, CV_8UC1);
    imwrite(dest_path+ "gammaCorrection"+"_"+ to_string(fDegradation)+ ".jpg", dst);
    drawHistogram(dst, "gamma"+to_string(fDegradation));
}

//homework3..full scale histogram
int computeOutput(int x, int r1, int s1, int r2, int s2)
{
    float result;
//    if(0 <= x && x <＝ r1){
//        result = s1/r1 * x;
//    }else if(r1 <= x && x < r2){
//        result = ((s2 - s1)/(r2 - r1)) * (x - r1) + s1;
//    }else if(r2 <= x && x <= 255){
//        result = 255;
//    }
    result = x * (s2-s1)/(r2-r1);
    return (int)result;
}

void full_scale_stretch(Mat src) {
    Mat dst, hist;
    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };
    calcHist(&src, 1, 0, Mat(), hist, 1, &histSize, &histRange);
    
    //find the rmin and rmax value
    int hist_min= 0, hist_max= 255 ;
    for(int i=0;i<256;i++){
        if(hist.at<float>(i)>0){
            hist_min = i;
            break;
        }
        
    }
    for(int i=0;i<256;i++){
        if(hist.at<float>(255-i)>0){
            hist_max = 255-i;
            break;
        }
        
    }
    
    src.convertTo(src, CV_32FC1);
    dst = Mat::zeros(src.rows, src.cols, CV_32FC1);
    for(int x = 0; x < src.rows; x++){
        for(int y = 0; y < src.cols; y++){
            dst.at<float>(x,y) = computeOutput(src.at<float>(x,y), hist_min, 0, hist_max, 255);
        }
    }
    
    dst.convertTo(dst, CV_8UC1);
    imwrite(dest_path+ "full-scale.jpg", dst);
    drawHistogram(dst, "fullScale");
    
}

//homework4
void hist_equalize(Mat src) {
    Mat dst;
    equalizeHist(src,dst);
    imwrite(dest_path+ "histogram_equalization.jpg", dst);
    drawHistogram(dst, "histEqualize");
}

//unsharping mask

void unsharping_mask(Mat src , bool unsharp, float kMask) {
    Mat dst, unsharp_mask, sharp_image;
    blur(src, dst, Size(5,5), Point(2,2));
    imwrite(dest_path+ "selfie_smoooth_image.jpg", dst);
    if(unsharp) {
        unsharp_mask = src - dst;
        sharp_image = src + kMask* unsharp_mask;
        imwrite(dest_path+ "unsharp_mask.jpg", unsharp_mask);
        imwrite(dest_path+ "selfie_sharp_image.jpg", sharp_image);
    }
}









