// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <algorithm>
// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <ctime>
// //#include "kcftracker.hpp"
// #include <dirent.h>
// #include "ECO.h"

// using namespace std;
// using namespace cv;
// //using namespace cv;

// static string WIN_NAME = "ECO-Tracker";
// bool gotBB = false;
// bool drawing_box = false;
// cv::Rect box;
// void mouseHandler(int event, int x, int y, int flags, void *param){
// 	switch (event){
// 	case cv::EVENT_MOUSEMOVE:
// 		if (drawing_box){
// 			box.width = x - box.x;
// 			box.height = y - box.y;
// 		}
// 		break;
// 	case cv::EVENT_LBUTTONDOWN:
// 		drawing_box = true;
// 		box = cv::Rect(x, y, 0, 0);
// 		break;
// 	case cv::EVENT_LBUTTONUP:
// 		drawing_box = false;
// 		if (box.width < 0){
// 			box.x += box.width;
// 			box.width *= -1;
// 		}
// 		if (box.height < 0){
// 			box.y += box.height;
// 			box.height *= -1;
// 		}
// 		gotBB = true;
// 		break;
// 	}
// }

// void drawBox(cv::Mat& image, cv::Rect box, cv::Scalar color, int thick){
// 	rectangle(image, cv::Point(box.x, box.y), cv::Point(box.x + box.width, box.y + box.height), color, thick);
// }

// String straddd(String str1,int inx,  String str2);


// int main(int argc, char**argv){

//     cv::Mat frame;
//     cv::Rect result;
//     cv::namedWindow(WIN_NAME);
//     cv::VideoCapture cap;
//     cap.open("/home/chengxin/Downloads/occlusion_half.mp4");

//     cv::namedWindow(WIN_NAME.c_str(),cv::WINDOW_AUTOSIZE);
//     cv::setMouseCallback(WIN_NAME.c_str(), mouseHandler, NULL);

//     cap >> frame;
//     if(frame.empty())
//     {
//         cout << "ERR frame" << endl;
//         return -1;
//     }

//     cv::Mat temp;
//     frame.copyTo(temp);

//     while (!gotBB)
//     {
//         drawBox(frame, box, cv::Scalar(0, 0, 255), 1);
//         cv::imshow(WIN_NAME, frame);
//         temp.copyTo(frame);
//         if (cv::waitKey(20) == 27)
//             break;
//     }

//     cv::setMouseCallback(WIN_NAME.c_str(), NULL, NULL);
//     ECO Eco;  
//     Eco.init(frame, box);

//     int a =0;
//     clock_t start, end;
//     start = clock();
//     while (cap.isOpened())
//     {   	

//         a++;
//         cap >> frame;

//         Eco.process_frame(frame);  
         
//     }
//     end = clock();
//     cout << "FPS:"  << a/((double)(end - start) / CLOCKS_PER_SEC) << endl;

//     cv::destroyAllWindows();

//     return 0;
// }




// String straddd(String str1,int inx,  String str2)
// {

//     for(int i = 0; i< 4; i++)
//     {
//         str1[inx] = str2[i];
//         cout << "str2 i" << inx << "  " << str2[i] << "  " <<str1 <<endl;
//         inx += 1;
//     }

//     String str3 = ".jpg";
//     for(int i = 0; i <4; i++)
//     {
//         str1[inx] = str3[i];
//         cout << "str3 i" << inx << "  " << str3[i] << "  " <<str1 <<endl;
//         inx += 1;
//     }
    
//     return str1;
// }