#pragma once

#ifndef POSTPROCESS_H
#define POSTPROCESS_H
#endif

#include<opencv2/opencv.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/dnn.hpp>
#include<opencv2/highgui.hpp>

cv::Rect postprocess(cv::Mat &frame, cv::Point p, std::vector<cv::Mat>& out, float CONFTHR, float NMSTHR, std::vector<std::string> classes);


// Draw the predicted bounding box
void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame, std::vector<std::string> classes);

//
//// Get the names of the output layers
//std::vector<cv::String> getOutputsNames(const cv::dnn::Net& net);


std::vector<std::string> getclass(std::string classpath);
