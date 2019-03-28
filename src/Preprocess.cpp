#include<opencv2/opencv.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/dnn.hpp>
#include<opencv2/highgui.hpp>
#include<fstream>

using namespace cv;
using namespace std;

vector<String> getOutputsNames(const dnn::Net& net);

void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame, vector<string> classes);


Rect postprocess(Mat &frame, Point p, vector<Mat>& out, float CONFTHR, float NMSTHR, vector<string> classes)
{
	cout << "point  " << p.x << p.y << endl;
	vector<int> classids;
	vector<float> conf;
	vector<Rect> boxes;

	for (int i = 0; i < out.size(); i++)
	{
		//��ȡԤ��bbox
		float* data = (float*)out[i].data;
		for (int j = 0; j < out[i].rows; ++j, data += out[i].cols)
		{
			Mat scores = out[i].row(j).colRange(5, out[i].cols);
			Point classIdpoint;
			double confidence;
			minMaxLoc(scores, 0, &confidence, 0, &classIdpoint);
			if (confidence > CONFTHR)
			{
				int x = (int)(data[0] * frame.cols);
				int y = (int)(data[1] * frame.rows);
				int w = (int)(data[2] * frame.cols);
				int h = (int)(data[3] * frame.rows);
				int l = x - w / 2;
				int t = y - h / 2;
				if (Rect(x, y, w + 40, h + 40).contains(p))
				{
					classids.push_back(classIdpoint.x);
					conf.push_back((float(confidence)));
					boxes.push_back(Rect(l, t, w, h));
				}


			}
		}
	}
	vector<int> indices;
	dnn::NMSBoxes(boxes, conf, CONFTHR, NMSTHR, indices);
	
	Rect rect;
	int max_conf = 0;
	for (int i = 0; i < indices.size(); i++)
	{

		int idx = indices[i];
		if (conf[idx] > max_conf)
		{
			cout << "get truc " << conf[idx] << " " << boxes[idx].x << " " << boxes[idx].y << endl;
			max_conf = conf[idx];
			rect = boxes[idx];
		}
		//Rect box = boxes[idx];
		//drawPred(classids[idx], conf[idx], box.x, box.y, box.x + box.width, box.y + box.height, frame, classes);
	}
	return rect;
}

// Draw the predicted bounding box
void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame, vector<string> classes)
{
	//Draw a rectangle displaying the bounding box
	rectangle(frame, Point(left, top), Point(right, bottom), Scalar(0, 0, 255));

	//Get the label for the class name and its confidence
	string label = format("%.2f", conf);
	if (!classes.empty())
	{
		CV_Assert(classId < (int)classes.size());
		label = classes[classId] + ":" + label;
	}

	//Display the label at the top of the bounding box
	int baseLine;
	Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
	top = max(top, labelSize.height);
	putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255));
}

//// Get the names of the output layers
//vector<String> getOutputsNames(const dnn::Net& net)
//{
//	static vector<String> names;
//	if (names.empty())
//	{
//		//Get the indices of the output layers, i.e. the layers with unconnected outputs
//		vector<int> outLayers = net.getUnconnectedOutLayers();
//
//		//get the names of all the layers in the network
//		vector<String> layersNames = net.getLayerNames();
//
//		// Get the names of the output layers in names
//		names.resize(outLayers.size());
//		for (size_t i = 0; i < outLayers.size(); ++i)
//			names[i] = layersNames[outLayers[i] - 1];
//	}
//	return names;
//}

vector<string> getclass(string classpath)
{
	string line;
	vector<string> classs;
	ifstream ifs(classpath.c_str());

	while (getline(ifs, line))
		classs.push_back(line);
	return classs;
} 