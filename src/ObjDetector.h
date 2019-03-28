#pragma once

#ifndef OBJDETECTOR_H
#define OBJDETECTOR_H
#endif

#include<iostream>
#include<opencv2/dnn.hpp>

class ObjDetector
{
private:
	std::string model;
	std::string cfg;
	void loadModel();

public:
	cv::dnn::Net net;
	ObjDetector();
	ObjDetector(std::string model, std::string cfg);
	virtual ~ObjDetector() {};

	std::vector<cv::Mat> predict(cv::Mat frame);

};