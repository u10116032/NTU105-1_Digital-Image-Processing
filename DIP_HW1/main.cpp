//
//  main.cpp
//  Practice
//
//  Created by 王瀚磊 on 2016/7/16.
//  Copyright © 2016年 王瀚磊. All rights reserved.
//

#include <opencv.hpp>
#include <highgui.hpp>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string>


using namespace cv;
using namespace std;

void bilinearInterpolation(Mat src, Mat &dst, float resize_ratio);
void bicubicInterpolation(Mat src, Mat &dst, float resize_ratio);
Vec3f cubicInterpolation(Vec3f p[4], float dx);

string output_path;

 // ./Pratice [input_image_absolute_path] [output_imag_absolute_path]

int main(int argc, char** argv)
{
    Mat src,dst;
    output_path = argv[2];
    src = imread(argv[1],CV_LOAD_IMAGE_COLOR);
    
    bilinearInterpolation(src, dst, 0.3);
    bilinearInterpolation(src, dst, 3);
    bilinearInterpolation(src, dst, 18);

    bicubicInterpolation(src, dst, 0.3);
    bicubicInterpolation(src, dst, 3);
    bicubicInterpolation(src, dst, 18);


    
    return 0;
}

void  bilinearInterpolation(Mat src, Mat &dst, float resize_ratio){
    dst = Mat::zeros(ceil(src.rows*resize_ratio),ceil(src.cols*resize_ratio),CV_8UC3);
    for(int i=0;i<dst.rows;i++){
        for(int j=0;j<dst.cols;j++){
            float px = (float)i/(float)resize_ratio;//src對應x座標
            float py = (float)j/(float)resize_ratio;//src對應y座標
            int x = floor(px); //左上角x
            int y = floor(py); //左上角y
            float dx = px-floor(px);
            float dy = py-floor(py);
            
            
            //BGR
            dst.at<Vec3b>(i,j) = src.at<Vec3b>(x,y)*(1-dx)*(1-dy) +
                                    src.at<Vec3b>(min(x+1,src.rows-1), y)*dx*(1-dy) +
                                    src.at<Vec3b>(x, min(y+1,src.cols-1))*(1-dx)*dy +
                                    src.at<Vec3b>(min(x+1,src.rows-1), min(y+1,src.cols-1))*dx*dy;
            
        }
    }
    
    cv::imwrite(output_path+"bilinear" +to_string(resize_ratio)+".jpg",dst);

}

void bicubicInterpolation(Mat src, Mat &dst, float resize_ratio){
    src.convertTo(src, CV_32FC3);
    dst = Mat::zeros(ceil(src.rows*resize_ratio),ceil(src.cols*resize_ratio),CV_32FC3);
    for(int i=0;i<dst.rows;i++){
        for(int j=0;j<dst.cols;j++){
            float px = (float)i/(float)resize_ratio;//src對應x座標
            float py = (float)j/(float)resize_ratio;//src對應y座標
            int x = floor(px); //左上角x
            int y = floor(py); //左上角y
            float dx = px-floor(px);
            float dy = py-floor(py);

            
            Vec3f vec3f0[4] = {
                src.at<Vec3f>((int)max(x-1,0),(int)max(y-1,0)),
                src.at<Vec3f>((int)max(x-1,0),(int)y),
                src.at<Vec3f>((int)max(x-1,0),(int)min(y+1,src.cols-1)),
                src.at<Vec3f>((int)max(x-1,0),(int)min(y+2,src.cols-1))
                };
            
            Vec3f vec3f1[4] = {
                src.at<Vec3f>((int)x,(int)max(y-1,0)),
                src.at<Vec3f>((int)x,(int)y),
                src.at<Vec3f>((int)x,(int)min(y+1,src.cols-1)),
                src.at<Vec3f>((int)x,(int)min(y+2,src.cols-1))
                };
            
            Vec3f vec3f2[4] = {
                src.at<Vec3f>((int)min(x+1,src.rows-1),(int)max(y-1,0)),
                src.at<Vec3f>((int)min(x+1,src.rows-1),(int)y),
                src.at<Vec3f>((int)min(x+1,src.rows-1),(int)min(y+1,src.cols-1)),
                src.at<Vec3f>((int)min(x+1,src.rows-1),(int)min(y+2,src.cols-1))
                };
            
            Vec3f vec3f3[4] = {
                src.at<Vec3f>((int)min(x+2,src.rows-1),(int)max(y-1,0)),
                src.at<Vec3f>((int)min(x+2,src.rows-1),(int)y),
                src.at<Vec3f>((int)min(x+2,src.rows-1),(int)min(y+1,src.cols-1)),
                src.at<Vec3f>((int)min(x+2,src.rows-1),(int)min(y+2,src.cols-1))
                };
            
            
            
            Vec3f vec3fY[4] = {cubicInterpolation(vec3f0,dy),cubicInterpolation(vec3f1,dy),cubicInterpolation(vec3f2,dy),cubicInterpolation(vec3f3,dy)};
            
            dst.at<Vec3f>(i,j) = cubicInterpolation(vec3fY, dx);
            
        }
    }
    dst.convertTo(dst, CV_8UC3);

    cv::imwrite(output_path+"bicubic" +to_string(resize_ratio)+".jpg",dst);

    

}

Vec3f cubicInterpolation(Vec3f p[4], float dx){
    return p[1] + 0.5 * dx*(p[2] - p[0] + dx*(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + dx*(3.0*(p[1] - p[2]) + p[3] - p[0])));
}


    
    
    
    
    
    
    
    
        
    
