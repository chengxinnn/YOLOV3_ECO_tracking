/*
C++ Felzenszwalb HOG extractor

This repository is meant to provide an easy-to-use implementation of the Felzenszwalb HOG features extractor.
This approach followed the one presented by Felzenszwalb, Pedro F., et al. "Object detection with discriminatively trained part-based models." Pattern Analysis and Machine Intelligence, IEEE Transactions on 32.9 (2010): 1627-1645.
The OpenCV library have only the original HOG, proposed by Dalal and Triggs. However, the Latent SVM OpenCV implementation have its own FHOG extractor. This code allows you to use it without having do deal with Latent SVM objects.

To run the code you need OpenCV library.

Author: Joao Faro
Contacts: joaopfaro@gmail.com
*/

#include "FHOG.hpp"

using namespace std;
using namespace cv;

HogFeature::HogFeature(){

	_cell_size = 4;
	_scale = 1;

}

HogFeature::HogFeature(uint cell_size, uint scale){

	_cell_size = cell_size;
	_scale = scale;

}

HogFeature::~HogFeature(){

	freeFeatureMapObject(&_map);

}

HogFeature* HogFeature::clone() const{

	return new HogFeature(*this);

}

cv::Mat HogFeature::getFeature(cv::Mat image){

	//image = image.t();

	_tmpl_sz.width = _scale * image.cols;
	_tmpl_sz.height = _scale * image.rows;
	//cout << "cell size " << _cell_size << endl;
	//cout << "_tmpl_sz.width " <<_tmpl_sz.width << endl;
	//cout <<"_tmpl_sz.height " << _tmpl_sz.height << endl;
	// Round to cell size and also make it even
	if (int(_tmpl_sz.width / ( _cell_size)) % 2 == 0)
	{
		_tmpl_sz.width = (((int)(_tmpl_sz.width / (2 * _cell_size))) * 2 * _cell_size) + _cell_size * 2;
		_tmpl_sz.height = (((int)(_tmpl_sz.height / (2 * _cell_size))) * 2 * _cell_size) + _cell_size * 2;
	}
	else
	{
		_tmpl_sz.width = (((int)(_tmpl_sz.width / (2 * _cell_size))) * 2 * _cell_size) + _cell_size * 3;
		_tmpl_sz.height = (((int)(_tmpl_sz.height / (2 * _cell_size))) * 2 * _cell_size) + _cell_size * 3;
	}


	//image.convertTo(image, CV_32F, 1 / 255.f);
	if (image.cols != _tmpl_sz.width || image.rows != _tmpl_sz.height) {
		resize(image, image, _tmpl_sz);
	}
	//cv::Rect extracted_roi;
	//extracted_roi.height = _tmpl_sz.height + _cell_size * 2;
	//extracted_roi.width = _tmpl_sz.width + _cell_size * 2;
	//extracted_roi.x = _cell_size+1;
	//extracted_roi.y = _cell_size+1;
	// Add extra cell filled with zeros around the image
	//cout << "_tmpl_sz.height " << _tmpl_sz.height << endl;
	//cv::imshow("img", featurePaddingMat);
	//cv::waitKey(0);
	//image.copyTo(featurePaddingMat.rowRange(_cell_size, _cell_size+_tmpl_sz.height).colRange(_cell_size, _cell_size+_tmpl_sz.width));
	//image.copyTo(featurePaddingMat);
	cv::Mat featurePaddingMat(_tmpl_sz.height + _cell_size * 2, _tmpl_sz.width + _cell_size * 2, CV_32FC3, cvScalar(0, 0, 0));

	image.copyTo(featurePaddingMat);
	//cout << image<< endl;
	// HOG feature
	//cv::namedWindow("img", CV_WINDOW_NORMAL);
	//cv::imshow("img" , image);
	//cv::waitKey(0);
	//cv::namedWindow("feature", CV_WINDOW_NORMAL);
	//cv::imshow("feature" , featurePaddingMat);
	//cv::waitKey(0);

	IplImage zz = featurePaddingMat;
	getFeatureMaps(&zz, _cell_size, &_map);
	normalizeAndTruncate(_map, 0.2f);
	PCAFeatureMaps(_map);
	_featuresMap = cv::Mat(cv::Size(_map->sizeX, _map->sizeY), CV_32FC(_map->numFeatures), _map->map);  // Procedure do deal with cv::Mat multichannel bug
	_featuresMap = _featuresMap.clone();
	freeFeatureMapObject(&_map);
	return _featuresMap;

}
