#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<dirent.h>

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/dnn/dnn.hpp>

#include"ECO.h"
#include"ObjDetector.h"
#include"PostProcess.h"

using namespace  cv;
using namespace std;

//�����������
float CONFTHR = 0.15;
float NMSTHR = 0.4;
int INWIDTH = 416;
int	INHEIGHT = 416;

//·��
const string VIDEO_PATH = "/home/chengxin/Downloads/scale_rotation_occlusion1.mp4";
const string CFG_PATH   = "/home/chengxin/Downloads/YOLO_DET/data/model/yolov3.cfg";
const string MODEL_PATH = "/home/chengxin/Downloads/YOLO_DET/data/model/yolov3.weights";
const string CLASS_PATH = "/home/chengxin/Downloads/YOLO_DET/data/model/coco.names";
const string OUT_FILE   = "/home/chengxin/Downloads/YOLO_DET/data/out.mp4";


//��������
vector<Mat>out;
Rect rect;
Point p;
bool istracking = false;
ObjDetector obd(MODEL_PATH, CFG_PATH);
vector<string> classes = getclass(CLASS_PATH);

void onMouse(int event, int x, int y, int flags, void*param);

int main(int argc, char** argv)
{
	ECO* eco = new ECO;

	VideoCapture cap;
	VideoWriter video;
	Mat frame, blob;

	string winname = "WINNAME ";
	namedWindow(winname, WINDOW_FULLSCREEN);
    //setWindowProperty(winname, WND_PROP_AUTOSIZE, WINDOW_AUTOSIZE);        
    //moveWindow(winname,50,50);
	setMouseCallback(winname, onMouse, (void*)&frame);
    

	ifstream ifs(VIDEO_PATH);
	if (!ifs) throw("could not read the video properly");
	cap.open(VIDEO_PATH);

	while (true)
	{
		cap >> frame;
		if (frame.empty())
		{
			cout << "Done " << endl;
			break;
		}

		//One prediction
		if (out.size() > 0)
		{
	
			rect = postprocess(frame,p, out, CONFTHR, NMSTHR, classes);
			out.clear();
			cout << rect.x << " " << rect.y << " " << rect.width << " " << rect.height <<endl;

            //Capture the target
			if (rect.x > 0 |rect.y > 0)
			{
				if( eco == NULL & istracking == false)
				   eco = new ECO;
				if(eco)
				{
					cout << "eco exist" << "   ";
                	eco->init(frame, rect);
					cout << "eco init success" <<endl; 
				}
			    istracking = true;
			}
			
		}
		if(istracking)
		{
			if(eco)
				eco->process_frame(frame);
		}

		imshow(winname, frame);
		if (waitKey(20) == 'r')
		{
            istracking = false;
			delete eco;
			eco = NULL;
		}   
		if (waitKey(20) == 27)
			break;
	}

	cap.release();
	destroyAllWindows();

	return 0;
}

void onMouse(int event, int x, int y, int flags, void*param)
{
	Mat frame = *(Mat*)param;
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		p = Point(x, y);
		out = obd.predict(frame);
		break;
	default:
		break;
	}
}