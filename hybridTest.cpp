#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "Preditor.cpp"
#include "Hybrid.cpp"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]){
    string img_file ="../img.bin";
    VideoCapture cap("../../ducks.y4m");
    if(!cap.isOpened()){
        cout << "Error opening the video" <<endl;
    return 0;
    }
    int frame_count = cap.get(CAP_PROP_FRAME_COUNT);
    BitStream bse(img_file,"w");
    HybridEncoder h(&bse,8,3,3,4);
    Mat prev;
    cap >>prev;
    int i=1;
    while(i<frame_count){
        Mat curr;
        cap >> curr;
        if(curr.empty()){
            break;
        }
        h.Encode(prev,curr);
        if(i%3==0){
            prev=curr;
        }
        i++;
    }
    
    cap.release();
}