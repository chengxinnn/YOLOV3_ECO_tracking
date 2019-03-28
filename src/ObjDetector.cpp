#include"ObjDetector.h"

#include<iostream>
#include<opencv2/opencv.hpp>

ObjDetector::ObjDetector()
{
	this->model = "F:/project/weight_cfg/yolov3.weights";
	this->cfg = "F:/project/weight_cfg/yolov3.cfg";
}

ObjDetector::ObjDetector(std::string model, std::string cfg)
{
	this->model = model;
	this->cfg = cfg;
	this->loadModel();

}

void ObjDetector::loadModel()
{
	if ((this->model.find("weights") != -1) & (this->cfg.find("cfg") != -1))
	{
		try {
			this->net = cv::dnn::readNetFromDarknet(this->cfg, this->model);
		}
		catch (cv::Exception &ee) {
			std::cout << "warning Exception " << ee.what() << std::endl;
			if (net.empty())
			{
				std::cout << "could not load the net framework properly" << std::endl;
				return;
			}
		}
	}
	else
		std::cout << "Wrong input for network";

	this->net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
	this->net.setPreferableBackend(cv::dnn::DNN_TARGET_CPU);
	std::cout << "***loadmodel success" << std::endl;
}


std::vector<cv::Mat> ObjDetector::predict(cv::Mat frame)
{
	cv::Mat blob;

	resize(frame, frame, cv::Size(416, 416));
	blob = cv::dnn::blobFromImage(frame, 1 / 255.0, cv::Size(416, 416), cv::Scalar(0, 0, 0), true, false);


	this->net.setInput(blob, "data");
	std::cout << "has read pic" << std::endl;
	std::vector<cv::Mat> out;
	this->net.forward(out);
	std::cout << "Get predict outs!" << std::endl;
	return out;

}
