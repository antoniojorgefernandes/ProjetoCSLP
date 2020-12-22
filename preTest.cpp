#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "Preditor.cpp"
#include <iostream>
using namespace std;
int main(int argc, char* argv[]){
    string img_file ="../img.bin";
    BitStream bse(img_file,"w");
    int m=24;
    Preditor preditorP;
    VideoCapture cap("../../ducks.y4m");
    if(!cap.isOpened()){
        cout << "Error opening the video" <<endl;
    return 0;
    }
    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH); 
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    int frame_count = cap.get(CAP_PROP_FRAME_COUNT);
    bse.writeNBits(m,5);
    bse.writeNBits(frame_height,12);
    bse.writeNBits(frame_width,12);
    while(1){
        Mat frame;
        cap >> frame;
        if(frame.empty()){
            break;
        }
        preditorP.predict(frame,m,&bse);
    }
    bse.close();
    Preditor preditorR;
    BitStream bs(img_file,"r");
    m=bs.readNBits(5);
    int height = bs.readNBits(12);
    int width = bs.readNBits(12);
    while(frame_count--){
        int r = preditorR.reconstruct(&bs,m,height,width);
        if (r==-1){
            break;
        }
    }

    cap.release();
}