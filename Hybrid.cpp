#include <stdio.h>
using namespace std;
using namespace cv;

class Hybrid{
    public:


    Hybrid(BitStream *bs){
        this->bs=bs;
    }
    protected:
    BitStream *bs;
    int frame_num=1;
};


class HybridEncoder: Hybrid{
    public:
    HybridEncoder(BitStream *bs, int block_size, int search_area,int periodicity,int m):Hybrid(bs){
        this->block_size=block_size;
        this->search_area=search_area;
        this->periodicity=periodicity;
        this->m=m;
    }

    void Encode(Mat reference_frame, Mat current_frame){
        Mat tmp = Mat::zeros(Size(current_frame.cols, current_frame.rows), CV_8UC3);
        if(this->frame_num%this->periodicity==0){
            Preditor pred;
            pred.predict(current_frame,this->m,this->bs);
        }
        else{
            GolombEncoder g(this->bs,this->m);
            Mat reference;
            Mat current;
            cvtColor(reference_frame, reference,COLOR_BGR2YUV);
            cvtColor(current_frame, current,COLOR_BGR2YUV);
            Mat ref_yuv[3];
            split(reference, ref_yuv);
            Mat curr_yuv[3];
            split(current, curr_yuv);
            for(int row=0; row <reference.rows; row+=this->block_size){
                for (int col=0; col<reference.cols; col+=this->block_size){ 
                    int r=this->block_size;
                    Mat current_block(curr_yuv[0], Rect(col,row,r,r));
                    int x =max(row-this->search_area,0);
                    int y =max(col-this->search_area,0);
                    int k =this->block_size+this->search_area;
                    int j =this->block_size+this->search_area;
                    if(x+this->block_size+this->search_area>=reference.rows){
                        k=abs(reference.rows-x);
                    }
                    if(y+this->block_size+this->search_area>=reference.cols){
                        j=abs(reference.cols-y);
                    }
                    Mat ref_block(ref_yuv[0],Rect(y,x,k,j));
                    Point p(0,0);
                    int step=4;
                    int mini=257;
                    while(step!=1){
                        map<int,Point> tmp = closest_match(p,step,current_block,ref_block,mini);
                        for(auto &pair : tmp){
                            if (pair.first<mini){
                                mini=pair.first;
                                p=pair.second;
                                
                            }
                        }
                        step/=2;
                    }
                    int count=0;
                    int aux1=row;
                    for(int block_r=abs(p.x)+row;block_r<=min(abs(p.x)+row+this->block_size,reference.rows);block_r++){
                        int aux2=col;
                        for(int block_c=abs(p.y)+col;block_c<=min(abs(p.y)+col+this->block_size,reference.cols);block_c++){
                            tmp.at<Vec3b>(aux1,aux2)=reference_frame.at<Vec3b>(block_r,block_c);
                            aux2++;
                        }
                        aux1++;
                    }
                }
            }
            imshow("as",tmp);
            char asas = (char)waitKey(15);
        }
        this->frame_num++;
    }

    map<int,Point> closest_match(Point start,int step,Mat block, Mat reference,int mini){
        map<int,Point> ret;
        Point pnt;
        int min_block_dif=mini;
        int y_plus_step =min(start.y+step,reference.cols);
        int x_plus_step =min(start.x+step,reference.rows);
        int y_minus_step =max(start.y-step,0);
        int x_minus_step =max(start.x-step,0);
        vector<Point> search_points {start,
                                     Point(start.x,y_plus_step),
                                     Point(x_plus_step,start.y),
                                     Point(x_plus_step,y_plus_step),
                                     Point(x_minus_step,start.y),
                                     Point(start.x,y_minus_step),
                                     Point(x_minus_step,y_minus_step),
                                     Point(x_minus_step,y_plus_step),
                                     Point(x_plus_step,y_minus_step)
                                    };

        
        for (Point p : search_points){
            int rows=block.rows;
            int cols=block.cols;
            if(p.x+block.rows>=reference.rows){
                rows=abs(block.rows-p.x);
            }
            if(p.y+block.cols>=reference.cols){
                cols=abs(block.cols-p.y);
            }
            Mat ref_block(reference, Rect(p.x,p.y,rows,cols));
            min_block_dif = min(block_dif(ref_block,block), min_block_dif);
            pnt=p;
            if (min_block_dif==0){
                ret[min_block_dif]=pnt;
                return ret;
            }
        }
        ret[min_block_dif]=pnt;
    return ret;
    }

    int block_dif(Mat a, Mat b){
        int sum=0;
        for(int row=0; row <a.rows; row++){
            for (int col=0; col<a.cols; col++){
                int pixel_a = int(a.at<u_char>(row,col));
                int pixel_b = int(b.at<u_char>(row,col));
                sum+=abs(pixel_a-pixel_b);
            }
        }
        if(sum==0){
            return 0;
        }
        return sum/(a.rows*a.cols);
    }

    protected:
    int block_size,search_area,periodicity,m;
};

class HybridDecoder: Hybrid{
    public:
    HybridDecoder(BitStream *bs):Hybrid(bs){

    }

    void Decode(){
        
    }
};