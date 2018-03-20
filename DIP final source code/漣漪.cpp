//
//  main.cpp
//  DIP_HW5
//
//  Created by 張嘉豪 on 2016/10/8.
//  Copyright © 2016年 張嘉豪. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>
#include <time.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv/cxcore.h>

#define pi 3.1415926

using namespace std;
using namespace cv;

void Show_Image(Mat&, const string &);

int main()
{
    
    Mat inputFrame;
    VideoCapture videoCapture;
    videoCapture.open("/Users/liebe/Desktop/social.avi");
    
    Size videoSize = Size((int)videoCapture.get(CV_CAP_PROP_FRAME_WIDTH),(int)videoCapture.get(CV_CAP_PROP_FRAME_HEIGHT));
    
    VideoWriter writer;
    writer.open("/Users/liebe/Desktop/VideoTest.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, videoSize);
    
    Mat Img;
    
    float A,B;
    A=5; // 水波的擾動度（振幅），越大越看不出原來的樣子
    B=2; // 水波的密集程度（頻率），看得出原來的樣子，越大越密集
    
    
    float r0, r1, new_x, new_y;
    float p, q, x1, y1, x0, y0;
    float theta;
    
    namedWindow("show image",0);
    
    while(A>0 || B>1.8)
    {
        videoCapture >> Img;
        
        if(Img.empty()) break;
        
        Mat Img_out(Img.size(), CV_8UC3);
        Img.copyTo(Img_out);
        
        int width=Img.cols;
        int height=Img.rows;
        
        Point Center(width/2, height/2);
        
        for (int y=0; y<height; y++)
        {
            for (int x=0; x<width; x++)
            {
                y0 = Center.y-y;
                x0 = x-Center.x;
                theta = atan( y0/(x0+0.00001) ); // avoid x0 is equal to zero
                
                if( x0<0 ) theta = theta+pi;
                
                
                r0 = sqrt(x0*x0+y0*y0);
                r1 = r0 + A*width*0.01*sin(B*0.1*r0);
                
                new_x=r1*cos(theta);
                new_y=r1*sin(theta);
                
                new_x=Center.x+new_x;
                new_y=Center.y-new_y;
                
                if(new_x<0)         new_x=0;
                if(new_x>=width-1)  new_x=width-2;
                if(new_y<0)         new_y=0;
                if(new_y>=height-1) new_y=height-2;
                
                x1=(int)new_x;
                y1=(int)new_y;
                
                p=new_x-x1;
                q=new_y-y1;
                
                
                // 針對RGB做bilinear
                for (int k=0; k<3; k++)
                {
                    Img_out.at<Vec3b>(y, x)[k] = (1-p)*(1-q)*Img.at<Vec3b>(y1, x1)[k]+
                    (p)*(1-q)*Img.at<Vec3b>(y1,x1+1)[k]+
                    (1-p)*(q)*Img.at<Vec3b>(y1+1,x1)[k]+
                    (p)*(q)*Img.at<Vec3b>(y1+1,x1+1)[k];
                }
                
            }
        }

        writer.write(Img_out);
        
        if(waitKey(1) == 'q'){
            break;
        }
        
        if(A>1 && A<2) A-=0.02;
        else A-=0.026;
        
        if(B>1) B-=0.005;
    }
    waitKey(0);
}

void Show_Image(Mat& Image, const string& str)
{
    namedWindow(str.c_str(),CV_WINDOW_AUTOSIZE);
    imshow(str.c_str(), Image);
    
}







