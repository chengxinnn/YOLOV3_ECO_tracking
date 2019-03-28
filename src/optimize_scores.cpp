#include"optimize_scores.h"

void optimize_scores::compute_scores()
{
	//std::vector<cv::Mat> sampled_scores = sample_fs(scores_fs);
	sample_fs(scores_fs);
}

using namespace std;
std::vector<cv::Mat> optimize_scores::sample_fs(const std::vector<cv::Mat>& xf, cv::Size grid_sz)
{	//std::cout << xf[0].rows<< xf[0].cols<< "  xfsize"<<std::endl;
	std::vector<cv::Mat> sampled_scores;
	for (size_t i = 0; i < xf.size(); ++i)
	{
		int area = xf[i].size().area();
		cv::Mat tmp = fftd(fftshift(xf[i], 1, 1, 1), 1);
		sampled_scores.push_back(real(tmp)* area);   // only real part shall be stored
	}

	// to store the position of maximum value of response
	std::vector<int> row, col; 
	std::vector<float> 	init_max_score;
	for (size_t i = 0; i < xf.size(); ++i)
	{
		cv::Point pos;
		cv::minMaxLoc(sampled_scores[i], NULL, NULL, NULL, &pos);
		row.push_back(pos.y);
		//cout << "y " << pos.y << endl;
		//cout << "x " << pos.x << endl;
		//cout << "scores " << sampled_scores[i].at<float>(pos.y, pos.x) << endl;
		col.push_back(pos.x);
		init_max_score.push_back(sampled_scores[i].at<float>(pos.y, pos.x));
	}
	
	// Shift and rescale the coordinate system to [-pi, pi]
	int h = xf[0].rows, w = xf[0].cols;
	std::vector<float> max_pos_y, max_pos_x, init_pos_y, init_pos_x;
	for (size_t i = 0; i < row.size(); ++i)
	{
		max_pos_y.push_back( (row[i] + (h - 1) / 2) %  h - (h - 1) / 2);
		max_pos_y[i] *= 2 * CV_PI / h;
		max_pos_x.push_back( (col[i] + (w - 1) / 2) %  w - (w - 1) / 2); 
		max_pos_x[i] *= 2 * CV_PI / w;
	}
	init_pos_y = max_pos_y; init_pos_x = max_pos_x;
	std::vector<float> ky, kx, ky2, kx2;
	for (int i = 0; i < h; ++i)
	{
		ky.push_back(i - (h - 1) / 2);
		ky2.push_back(ky[i] * ky[i]);
	}
	for (int i = 0; i < w; ++i)
	{
		kx.push_back(i - (w - 1) / 2);
		kx2.push_back(kx[i] * kx[i]);
	}

	std::vector<cv::Mat> exp_iky, exp_ikx;
	for (int i = 0; i < xf.size(); ++i)
	{
		cv::Mat tempy(1, h, CV_32FC2), tempx(w, 1, CV_32FC2);
		for (int y = 0; y < h; ++y)
			tempy.at<cv::Vec<float, 2> >(0, y) = cv::Vec<float, 2>(cos(ky[y] * max_pos_y[i]), sin(ky[y] * max_pos_y[i]));
		for (int x = 0; x < w; ++x)
			tempx.at<cv::Vec<float, 2> >(x,0) = cv::Vec<float, 2>(cos(kx[x] * max_pos_x[i]), sin(kx[x] * max_pos_x[i]));
		exp_iky.push_back(tempy);
		exp_ikx.push_back(tempx);
	}

	cv::Mat kyMat(1, h, CV_32FC1, &ky[0]), kxMat(w, 1, CV_32FC1, &kx[0]);
	cv::Mat ky2Mat(1, h, CV_32FC1, &ky2[0]), kx2Mat(w, 1, CV_32FC1, &kx2[0]);

	for (int  ite = 0; ite < iterations; ++ite)
	{
		// Compute gradient
		std::vector<cv::Mat> ky_exp_ky, kx_exp_kx, y_resp, resp_x, grad_y, grad_x;
		std::vector<cv::Mat> ival, H_yy, H_xx, H_xy, det_H;
		for (int i = 0; i < xf.size(); i++)
		{
			ky_exp_ky.push_back(complexMultiplication( exp_iky[i] ,kyMat));
			kx_exp_kx.push_back(complexMultiplication( exp_ikx[i] ,kxMat));

			y_resp.push_back(exp_iky[i] * scores_fs[i]);

			resp_x.push_back(scores_fs[i] * exp_ikx[i]);

			grad_y.push_back(-1 * FFTTools::imag(ky_exp_ky[i] * resp_x[i]));
			grad_x.push_back(-1 * FFTTools::imag(y_resp[i] * kx_exp_kx[i]));

			// Compute Hessian
			ival.push_back(exp_iky[i] * resp_x[i]);
			std::vector<cv::Mat> tmp;
			cv::split(ival[i], tmp);
			std::vector<cv::Mat> tmp3;
			tmp3.push_back(-1 * tmp[1]);
			tmp3.push_back(tmp[0]);
			cv::merge(tmp3, ival[i]);

			H_yy.push_back(real(-1 * complexMultiplication(real2complx(ky2Mat), exp_iky[i]) * resp_x[i] + ival[i]));
			H_xx.push_back(real(-1 * y_resp[i] * complexMultiplication(exp_ikx[i], kx2Mat) + ival[i]));
			H_xy.push_back(real(-1 * ky_exp_ky[i] * (scores_fs[i] * kx_exp_kx[i])));

			det_H.push_back(H_yy[i].mul(H_xx[i]) - H_xy[i].mul(H_xy[i]));
		
			// Compute new position using newtons method
			cv::Mat tmp1, tmp2;
			cv::divide(H_xx[i].mul(grad_y[i]) - H_xy[i].mul(grad_x[i]), det_H[i], tmp1);
			max_pos_y[i] -= tmp1.at<float>(0, 0);
			cv::divide(H_yy[i].mul(grad_x[i]) - H_xy[i].mul(grad_y[i]), det_H[i], tmp2);
			max_pos_x[i] -= tmp2.at<float>(0, 0);

			// Evaluate maximum
			cv::Mat tempy(1, h, CV_32FC2), tempx(w, 1, CV_32FC2);
			for (int y = 0; y < h; ++y)
				tempy.at<cv::Vec<float, 2> >(0, y) = cv::Vec<float, 2>(cos(ky[y] * max_pos_y[i]), sin(ky[y] * max_pos_y[i]));
			for (int x = 0; x < w; ++x)
				tempx.at<cv::Vec<float, 2> >(x, 0) = cv::Vec<float, 2>(cos(kx[x] * max_pos_x[i]), sin(kx[x] * max_pos_x[i]));
			exp_iky[i] = tempy;
			exp_ikx[i] = tempx;
		}
	}

	std::vector<float> max_score;
	for (size_t i = 0; i < sampled_scores.size(); ++i)
	{
		float new_scores = real(exp_iky[i] * scores_fs[i] * exp_ikx[i]).at<float>(0, 0);
		if (new_scores > init_max_score[i])  // org: new_scores > init_max_score[i]
		{
			max_score.push_back(new_scores);
		}
		else
		{
			max_score.push_back(init_max_score[i]);
			max_pos_y[i] = init_pos_y[i];
			max_pos_x[i] = init_pos_x[i];
		}
			
	}
	 

	std::vector<float>::iterator pos = max_element(max_score.begin(), max_score.end());
	scale_ind = pos - max_score.begin();

	disp_row = (fmod(max_pos_y[scale_ind] + CV_PI, CV_PI * 2.0) - CV_PI) / (CV_PI * 2.0) * h;
	disp_col = (fmod(max_pos_x[scale_ind] + CV_PI, CV_PI * 2.0) - CV_PI) / (CV_PI * 2.0) * w;
	return sampled_scores;
}
