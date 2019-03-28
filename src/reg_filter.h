#ifndef REG_FILTER
#define REG_FILTER

#include <opencv2/features2d/features2d.hpp>
//#include <core\core.hpp>
#include <opencv2/opencv.hpp>
#include "params.hpp"
#include "fftTool.h"

#endif 

using FFTTools::fftd;
using FFTTools::magnitude;

extern cv::Mat  get_reg_filter(cv::Size sz, cv::Size2f target_sz, const eco_params& params);
