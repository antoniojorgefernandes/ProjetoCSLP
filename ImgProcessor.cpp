#include <stdint.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv; 
//!Reproduz um video a partir do nome ou mostra uma imagem
void PlayVideo(string NomeFicheiro){

  VideoCapture cap(NomeFicheiro);
    if(!cap.isOpened()){
  cout << "Error opening the video" <<endl;
    return ;
  }
  int count=0;
  while(1){
    count++;
    Mat frame;
    cap >> frame;
    if(frame.empty()){
      break;
    }
    imshow("Img",frame);
    if (count == 1){
      waitKey(0);
    }
    char c =(char)waitKey(15);
    if(c==27){
      break;
    }
  }
  cap.release();

  destroyAllWindows();

}
//!Reproduz um video ou mostra uma imagem a preto e branco
void grayScaleVideo(string NomeFicheiro){

  VideoCapture cap(NomeFicheiro);
    if(!cap.isOpened()){
  cout << "Error opening the video" <<endl;
    return ;
  }
  int count=0;
  while(1){
    count++;
    Mat frame;
    cap >> frame;
    if(frame.empty()){
      break;
    }

    Mat dst;
    cvtColor( frame, frame, COLOR_BGR2GRAY );
    equalizeHist(frame, dst );
    imshow("Greyscale",dst);

    if (count == 1){
      waitKey(0);
    }
    char c =(char)waitKey(15);
    if(c==27){
      break;
    }
    dst.release();
  }
  cap.release();

  destroyAllWindows();
}

//!Coloca watermark no canto superior esquerdo
void waterMark(string NomeFicheiro){

  VideoCapture cap(NomeFicheiro);
    if(!cap.isOpened()){
  cout << "Error opening the video" <<endl;
    return ;
  }
  int count =0;
  while(1){
    count++;
    Mat frame;
    cap >> frame;
    if(frame.empty()){
      break;
    }

    putText(frame, "92880", Point(100,100) , FONT_HERSHEY_SCRIPT_COMPLEX, 3, Scalar(0, 0, 0), 0.1);
    imshow("Frame",frame);
    if (count == 1){
      waitKey(0);
    }
    char c =(char)waitKey(15);
    if(c==27){
      break;
    }
  }
  cap.release();

  destroyAllWindows();
}
//!Reproduz o video ou mostra a imagem no formato YUV e mostra os diferentes canais
void openAsHSV(string NomeFicheiro){
  
  VideoCapture cap(NomeFicheiro);
    if(!cap.isOpened()){
  cout << "Error opening the video" <<endl;
    return ;
  }
  int count =0;
  while(1){
    count++;
    Mat frame;
    cap >> frame;
    if(frame.empty()){
      break;
    }
    Mat output;
    cvtColor(frame, output, COLOR_BGR2HSV);
    
    Mat HSV[3];
    split(output, HSV);
    namedWindow("H",(500,500));
    namedWindow("S",(500,500));
    namedWindow("V",(500,500));
    namedWindow("HSV", (500,500));
    moveWindow("H",10,10);
    moveWindow("S",400,10);
    moveWindow("V",10,400);
    moveWindow("HSV",400,400);
    imshow("HSV",output);
    imshow("H",HSV[0]);
    imshow("S",HSV[1]);
    imshow("V",HSV[2]);
    if (count == 1){
      waitKey(0);
    }
    char c =(char)waitKey(15);
    if(c==27){
      break;
    }
  }
  cap.release();

  destroyAllWindows();
}
//!Reproduz o video ou mostra a imagem no formato YUV e mostra os diferentes canais
void openAsYUV(string NomeFicheiro){
  
  VideoCapture cap(NomeFicheiro);
    if(!cap.isOpened()){
  cout << "Error opening the video" <<endl;
    return ;
  }
  int count=0;
  while(1){
    count++;
    Mat frame;
    cap >> frame;
    if(frame.empty()){
      break;
    }
    Mat output;
    cvtColor(frame, output,COLOR_BGR2YUV);
    Mat YUV[3];
    split(output, YUV);
    namedWindow("Y",(500,500));
    namedWindow("U",(500,500));
    namedWindow("V",(500,500));
    namedWindow("YUV", (500,500));
    moveWindow("Y",10,10);
    moveWindow("U",400,10);
    moveWindow("V",10,400);
    moveWindow("YUV",400,400);
    imshow("YUV",output);
    imshow("Y",YUV[0]);
    imshow("U",YUV[1]);
    imshow("V",YUV[2]);
    if (count == 1){
      waitKey(0);
    }
    char c =(char)waitKey(15);
    if(c==27){
      break;
    }
  }
  cap.release();

  destroyAllWindows();
}

//!Mostra o color histotram da imagem. No caso de um video mostra o histogram por frame
void colorHistogram(string NomeFicheiro){
  VideoCapture cap(NomeFicheiro);
  if(!cap.isOpened()){
    cout << "Error opening the video" <<endl;
    return ;
  }

  int count=
  0;
  while(1){
    count++;
    Mat frame;
    cap >> frame;
    if(frame.empty()){
      break;
    }
    vector<Mat> channels;
    split( frame, channels);
    int histSize = 256;
    float range[] = { 0, 256 }; 
    const float* histRange = { range };
    Mat b_hist, g_hist, r_hist;
    calcHist( &channels[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, true, false );
    calcHist( &channels[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, true, false );
    calcHist( &channels[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, true, false );
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 255,255,255) );
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    for( int i = 1; i < histSize; i++ )
    {
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
        Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
          Scalar( 255, 0, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
        Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
     
          Scalar( 0, 255, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
        Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
          Scalar( 0, 0, 255), 2, 8, 0  );
    }
    imshow("calcHist", histImage );
    if (count == 1){
      waitKey(0);
    }
    char c =(char)waitKey(15);
    if(c==27){
      break;
    }
  }
  cap.release();
  destroyAllWindows();
}


//!desfoca um video ou imagem
void blur(string NomeFicheiro){
   
  VideoCapture cap(NomeFicheiro);
    if(!cap.isOpened()){
  cout << "Error opening the video" <<endl;
    return ;
  }
  int count=0;
  while(1){
    count++;
    Mat frame;
    cap >> frame;
    if(frame.empty()){
      break;
    }

    Mat output =Mat::zeros(frame.rows, frame.cols, CV_8UC3);
    for ( int i = 1; i < 31; i = i + 2 )
    {
        GaussianBlur( frame, output, Size( i,i ), 0, 0 );
    }
    imshow("Img",output);
    if (count == 1){
      waitKey(0);
    }
    char c =(char)waitKey(15);
    if(c==27){
      break;
    }
  }
  cap.release();

  destroyAllWindows();

}

void threshold(string NomeFicheiro){
  
  VideoCapture cap(NomeFicheiro);
  if(!cap.isOpened()){
  cout << "Error opening the video" <<endl;
    return ;
  }
  int count=0;
  while(1){
    count++;
    Mat frame;
    cap >> frame;
    if(frame.empty()){
      break;
    }
  cvtColor( frame, frame, COLOR_BGR2GRAY );
  imshow("frame1", frame);
  threshold( frame, frame, 115,255, 3 );
  imshow( "frame", frame );
  if (count == 1){
      waitKey(0);
    }
    char c =(char)waitKey(15);
    if(c==27){
      break;
    }
  }
  cap.release();

  destroyAllWindows();

}

void dilation(string NomeFicheiro){

  VideoCapture cap(NomeFicheiro);
  if(!cap.isOpened()){
  cout << "Error opening the video" <<endl;
    return ;
  }
  int count=0;
  while(1){
    count++;
    Mat frame;
    cap >> frame;
    if(frame.empty()){
      break;
    }
  Mat element = getStructuringElement( MORPH_RECT,
                       Size( 2*5 + 1, 2*5+1 ),
                       Point( 5, 5 ) );
  dilate( frame, frame,element);
  imshow( "Dilation", frame );
  if (count == 1){
      waitKey(0);
    }
    char c =(char)waitKey(15);
    if(c==27){
      break;
    }
  }
  cap.release();

  destroyAllWindows();
}
void erosion(string NomeFicheiro){

  VideoCapture cap(NomeFicheiro);
  if(!cap.isOpened()){
  cout << "Error opening the video" <<endl;
    return ;
  }
  int count=0;
  while(1){
    count++;
    Mat frame;
    cap >> frame;
    if(frame.empty()){
      break;
    }
  Mat element = getStructuringElement( MORPH_RECT,
                       Size( 2*20 + 1, 2*20+1 ),
                       Point( 20, 20 ) );
  imshow( "Erosion", frame );
  if (count == 1){
      waitKey(0);
    }
    char c =(char)waitKey(15);
    if(c==27){
      break;
    }
  }
  cap.release();

  destroyAllWindows();
  
}
//!Converts from YUV444 to RGB
void convertYUV444TORGB(Mat frame){
  // VideoCapture cap(NomeFicheiro);
  // if(!cap.isOpened()){
  // cout << "Error opening the video" <<endl;
  //   return ;
  // }
  int count=0;
  while(1){
    count++;
    // Mat frame;
    // cap >> frame;

    if(frame.empty()){
      break;
    }
  Mat RGBMat(frame.rows, frame.cols, CV_8UC3);
  for(int i = 0; i < frame.rows; i++) {
    for(int j = 0; j < frame.cols; j++) {
        //get bgr pixel 
        
        Vec3b YCbCr = frame.at<Vec3b>(i, j);

        float Y = YCbCr[0];
        float Cb = YCbCr[1];
        float Cr = YCbCr[2];          

        int R = (int) (Y + 1.40200 * (Cr - 0x80));
        int G = (int) (Y - 0.34414 * (Cb - 0x80) - 0.71414 * (Cr - 0x80));
        int B = (int) (Y + 1.77200 * (Cb - 0x80));
        if(R<0){
          R=0;
        }
        else if(R>255){
          R=255;
        }
        if(G<0){
          G=0;
        }
        else if(G>255){
          G=255;
        }
        if(B<0){
          B=0;
        }
        else if(B>255){
          B=255;
        }
        
        Vec3b RGB(B,G,R);
        RGBMat.at<Vec3b>(i,j) = RGB;
    }
  }
  imshow("RGB",RGBMat);
  if (count == 1){
      waitKey(0);
    }
    char c =(char)waitKey(15);
    if(c==27){
      break;
    }
  }

  destroyAllWindows();

}
//!Converts from YUV422 to RGB
void convertYUV442TORGB(Mat frame){
  // VideoCapture cap(NomeFicheiro);
  // if(!cap.isOpened()){
  // cout << "Error opening the video" <<endl;
  //   return ;
  // }
  int count=0;
  while(1){
    count++;
    // Mat frame;
    // cap >> frame;

    if(frame.empty()){
      break;
    }
  Mat RGBMat(frame.rows, frame.cols, CV_8UC3);
  for(int i = 0; i < frame.rows; i++) {
    for(int j = 0, incr=2; j < frame.cols; j+=incr) {
        Vec3b YCbCr = frame.at<Vec3b>(i, j);
        float Y = YCbCr[0];
        float Cb = YCbCr[1];
        float Cr = YCbCr[2];       
        int R = (int) (Y + 1.40200 * (Cr - 0x80));
        int G = (int) (Y - 0.34414 * (Cb - 0x80) - 0.71414 * (Cr - 0x80));
        int B = (int) (Y + 1.77200 * (Cb - 0x80));
        if(R<0){
          R=0;
        }
        else if(R>255){
          R=255;
        }
        if(G<0){
          G=0;
        }
        else if(G>255){
          G=255;
        }
        if(B<0){
          B=0;
        }
        else if(B>255){
          B=255;
        }
        
        Vec3b RGB(B,G,R);
        RGBMat.at<Vec3b>(i,j) = RGB;
        if(j+1<=frame.cols){
          Vec3b YCbCr = frame.at<Vec3b>(i, j+1);
          Y = YCbCr[0];
          R = (int) (Y + 1.40200 * (Cr - 0x80));
          G = (int) (Y - 0.34414 * (Cb - 0x80) - 0.71414 * (Cr - 0x80));
          B = (int) (Y + 1.77200 * (Cb - 0x80));
          if(R<0){
            R=0;
          }
          else if(R>255){
            R=255;
          }
          if(G<0){
            G=0;
          }
          else if(G>255){
            G=255;
          }
          if(B<0){
            B=0;
          }
          else if(B>255){
            B=255;
          }
          Vec3b RGB(B,G,R);
          RGBMat.at<Vec3b>(i,j+1) = RGB;
        }

    }
  }
  imshow("RGB",RGBMat);
  if (count == 1){
      waitKey(0);
    }
    char c =(char)waitKey(15);
    if(c==27){
      break;
    }
  }

  destroyAllWindows();

}
//!Converts from RGB to YUV
void convertRGBTOYUV444(string NomeFicheiro){
  VideoCapture cap(NomeFicheiro);
  if(!cap.isOpened()){
  cout << "Error opening the video" <<endl;
    return ;
  }
  int count=0;
  while(1){
    count++;
    Mat frame;
    cap >> frame;

    if(frame.empty()){
      break;
    }
  Mat YMat(frame.rows, frame.cols, CV_8UC3);

  for(int i = 0; i < frame.rows; i++) {
    for(int j = 0; j < frame.cols; j++) {
        //get bgr pixel 
        Vec3b RGB = frame.at<Vec3b>(i, j);

        float B = RGB[0];
        float G = RGB[1];
        float R = RGB[2];          

        //actual conversion from BGR to YCrCb
        float Y = 0.299*R + 0.587*G + 0.114 * B;
        float Cb = 128 - 0.168736*R  - 0.331264* G + 0.5*B;
        float Cr = 128 + 0.5*R - 0.418688*G - 0.081312*B;
        //store into result image
        
        Vec3b ycbcr(Y,Cb,Cr);
        YMat.at<Vec3b>(i,j) = ycbcr;  
    }
  }
  convertYUV444TORGB(YMat);
  imshow("YCbCr",YMat);
  if (count == 1){
      waitKey(0);
    }
    char c =(char)waitKey(15);
    if(c==27){
      break;
    }
  }
  cap.release();

  destroyAllWindows();

}
//Converts from RGB to YUV 442
void convertRGBTOYUV442(string NomeFicheiro){
  VideoCapture cap(NomeFicheiro);
  if(!cap.isOpened()){
  cout << "Error opening the video" <<endl;
    return ;
  }
  int count=0;
  while(1){
    count++;
    Mat frame;
    cap >> frame;

    if(frame.empty()){
      break;
    }
  Mat YMat(frame.rows, frame.cols, CV_8UC1);
  Mat CbCrMat(frame.rows, frame.cols,CV_8UC2);
  for(int i = 0; i < frame.rows; i++) {
    for(int j = 0; j < frame.cols; j++) {
        //get bgr pixel 
        Vec3b RGB = frame.at<Vec3b>(i, j);

        float B = RGB[0];
        float G = RGB[1];
        float R = RGB[2];          

        //actual conversion from BGR to YCrCb
        int Y = 0.299*R + 0.587*G + 0.114 * B;
        int Cb = 128 - 0.168736*R  - 0.331264* G + 0.5*B;
        int Cr = 128 + 0.5*R - 0.418688*G - 0.081312*B;
        //store into result image
        if(j%2==0){
          Vec2b cbcr(Cb,Cr);
          CbCrMat.at<Vec2b>(i,j) = cbcr;
        }
        YMat.at<int>(i,j)=Y;
         
    }
  }
  vector<Mat>Channels(2);
  Channels[0]=YMat;
  Channels[1]=CbCrMat;
  Mat YMat1;
  merge(Channels, YMat1);
  convertYUV442TORGB(YMat1);
  imshow("YCbCr",YMat1);
  if (count == 1){
      waitKey(0);
    }
    char c =(char)waitKey(15);
    if(c==27){
      break;
    }
  }
  cap.release();

  destroyAllWindows();

}
//!Main function with a menu
int main( int argc, char** argv ) {


  while(1){
    int opt;
    string NomeFicheiro = "../lena.ppm";
    cout << "Escolha uma opção:"<<endl;
    cout << "1- Reproduzir video"<<endl;
    cout << "2- Watermark"<<endl;
    cout << "3- Transform into HSV"<<endl;
    cout << "4- Transform into YUV"<<endl;
    cout << "5- Color histogram"<<endl; 
    cout << "6- Convert to greyscale and apply histogram equalization"<<endl; 
    cout << "7- Blur"<<endl;
    cout << "8- Threshold"<<endl;
    cout << "9- Dilation"<<endl;
    cout << "10- Erosion"<<endl;
    cout << "11- Image gradients"<<endl; //https://docs.opencv.org/master/d2/d2c/tutorial_sobel_derivatives.html //https://docs.opencv.org/master/d5/db5/tutorial_laplace_operator.html
    cout << "12- Canny edge"<<endl;   //https://docs.opencv.org/master/da/d5c/tutorial_canny_detector.html  //https://docs.opencv.org/master/de/d09/tutorial_table_of_contents_contours.html
    cout << "13- Convert RGB to YUV"<<endl;
    cin>> opt;
    switch(opt){
      case 1 :
        PlayVideo(NomeFicheiro);
        break;
      case 2 :
        waterMark(NomeFicheiro);
        break;
      case 3:
        openAsHSV(NomeFicheiro);
        break;
      case 4:
        openAsYUV(NomeFicheiro);
        break;
      case 5:
        colorHistogram(NomeFicheiro);
        break;
      case 6:
        grayScaleVideo(NomeFicheiro);
        break;
      case 7:
        blur(NomeFicheiro);
        break;
      case 8:
        threshold(NomeFicheiro);
        break;
      case 9:
        dilation(NomeFicheiro);
        break;
      case 10:
        erosion(NomeFicheiro);
        break;
      case 13:
        convertRGBTOYUV442(NomeFicheiro);
      default:
        break;
    }
  }

  return 0;
}

