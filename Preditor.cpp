#include "BitStream.cpp"
#include <iostream>
#include "Golomb.cpp"
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/opencv.hpp>
using namespace std;
using namespace cv;


class Preditor{

    public:
    void predict(Mat frame,int m,BitStream *bs){
        GolombEncoder g(bs,m);
        vector<Mat> rgbChannels(3); //Mat rgbchannel[3];
	    split(frame, rgbChannels);
        int a,b,c,error,pred_value;
        for (int ch=0;ch<3;ch++){
            frame=rgbChannels[ch];
            Mat tmp = Mat::zeros(Size(frame.cols, frame.rows), CV_8UC1);
            for (int rows = 0; rows < frame.rows; rows++) {
                for (int cols = 0; cols <= frame.cols; cols++) {
                    if (rows==0 && cols==0){
                        a=0;
                        b=0;
                        c=0;
                    }
                    else if(rows==0 && cols>0){
                        a=0;
                        b=int(frame.at<u_char>(rows,cols-1));
                        c=0;
                    }
                    else if(cols==0 && rows>0){
                        a=int(frame.at<u_char>(rows-1,cols));
                        b=0;
                        c=0;
                    }
                    else{
                        a=int(frame.at<u_char>(rows,cols-1));
                        b=int(frame.at<u_char>(rows-1,cols));
                        c=int(frame.at<u_char>(rows-1,cols-1));
                    }
                    if(c<=min(a,b)){
                        pred_value=max(a,b);
                    }
                    else if(c>=max(a,b)){
                        pred_value=min(a,b);
                    }
                    else{
                        pred_value=a+b-c;
                    }
                    error=int(frame.at<u_char>(rows,cols))-pred_value;
                    tmp.at<u_char>(rows,cols)=u_char(pred_value);
                    g.encode(error);
                    g.encode(pred_value);
                    
                }
            }
        }
    }




    int reconstruct(BitStream *bs,int m,int rows, int cols){
        GolombDecoder g(bs,m);
        vector<Mat> channels;
        int error, pix;
        if (rows == 0 || cols ==0){
            return -1;
        }
        for (int ch=0;ch<3;ch++){
            Mat tmp = Mat::zeros(rows,cols, CV_8UC1);
            for (int row = 0; row < rows; row++) {
                for (int col = 0; col <= cols; col++) {
                    error=g.decode();
                    pix=g.decode();
                    pix+=error;
                    tmp.at<u_char>(row,col)=u_char(pix);
                }
            }
            channels.push_back(tmp);
        }
        Mat res;
        merge(channels,res);
        imshow("img",res);
        char c =(char)waitKey(3);
        return 0;
    }
};
