
#include "fftTool.h"

namespace FFTTools
{
	// Previous declarations, to avoid warnings

	template<typename _Tp>
	cv::Vec<_Tp, 2> conjugate(const cv::Vec<_Tp, 2>& v)
	{
		return cv::Vec<_Tp, 2>(v[0], -v[1]);
	}
	cv::Mat fftd(const cv::Mat& img_org, bool backwards)
	{
		cv::Mat img;
		img_org.copyTo(img);
		if (img.channels() == 1)
		{
			cv::Mat planes[] = { cv::Mat_<float>(img), cv::Mat_<float>::zeros(img.size()) };
			//cv::Mat planes[] = {cv::Mat_<double> (img), cv::Mat_<double>::zeros(img.size())};
			cv::merge(planes, 2, img);
		}
		cv::dft(img, img, backwards ? (cv::DFT_INVERSE | cv::DFT_SCALE) : 0);

		return img;

		/*#endif*/

	}
	cv::Mat fftr(const cv::Mat& img_org)
	{
		cv::Mat img;
		img_org.copyTo(img);
		if (img.channels() == 1)
		{
			cv::Mat planes[] = { cv::Mat_<float>(img), cv::Mat_<float>::zeros(img.size()) };
			//cv::Mat planes[] = {cv::Mat_<double> (img), cv::Mat_<double>::zeros(img.size())};
			cv::merge(planes, 2, img);
		}
		cv::dft(img, img,cv::DFT_ROWS);

		return img;

		/*#endif*/

	}

	cv::Mat real(cv::Mat img)
	{
		std::vector<cv::Mat> planes;
		cv::split(img, planes);
		return planes[0];
	}

	cv::Mat imag(cv::Mat img)
	{
		std::vector<cv::Mat> planes;
		cv::split(img, planes);
		return planes[1];
	}

	cv::Mat magnitude(cv::Mat img)
	{
		cv::Mat res;
		std::vector<cv::Mat> planes;
		cv::split(img, planes); // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
		if (planes.size() == 1) res = cv::abs(img);
		else if (planes.size() == 2) cv::magnitude(planes[0], planes[1], res); // planes[0] = magnitude
		else assert(0);
		return res;
	}

	cv::Mat complexMultiplication(cv::Mat a, cv::Mat b)
	{
		
		
		if (b.channels() == 1)     // for single channel image s
		{
			cv::Mat res;
			std::vector<cv::Mat> aa ;
			cv::split(a,aa);
			aa[0]= aa[0].mul(b);
			aa[1]= aa[1].mul(b);
			cv::merge(aa,res);
			return res;
		}
		cv::Mat res(a.rows, b.cols, CV_32FC2);

		//wangsen at���������޸ģ�����˷������޸ģ�opencv�����˵�˷���
		for (size_t i = 0; i < a.rows; i++)
		{
			for (size_t j = 0; j < b.cols; j++)
			{
				cv::Complex<float> ai(a.at<cv::Vec<float, 2> >(i, j)[0], a.at<cv::Vec<float, 2> >(i, j)[1]),
					bi(b.at<cv::Vec<float, 2> >(i, j)[0], b.at<cv::Vec<float, 2> >(i, j)[1]),c;
				c = ai * bi;
				res.at<cv::Vec<float, 2> >(i, j) = cv::Vec<float, 2>(c.re, c.im);
			}
		}





		return res;
	}


	float sum_conj(cv::Mat a, cv::Mat b)
	{
		float res = 0.0f;
		for (size_t i = 0; i < a.rows; i++)
		{
			for (size_t j = 0; j < b.cols; j++)
			{
				cv::Complex<float> ai(a.at<cv::Vec<float, 2> >(i, j)[0], a.at<cv::Vec<float, 2> >(i, j)[1]),
					bi(b.at<cv::Vec<float, 2> >(i, j)[0], - b.at<cv::Vec<float, 2> >(i, j)[1]),c;
				c = ai * bi;
				res += c.re;
			}
		}





		return res;
	}






	cv::Mat complexDivision(cv::Mat a, cv::Mat b)
	{

		std::vector<cv::Mat> pa;
		std::vector<cv::Mat> pb;
		cv::split(a, pa);
		cv::split(b, pb);

		cv::Mat divisor = 1. / (pb[0].mul(pb[0]) + pb[1].mul(pb[1]));

		std::vector<cv::Mat> pres;

		pres.push_back((pa[0].mul(pb[0]) + pa[1].mul(pb[1])).mul(divisor));
		pres.push_back((pa[1].mul(pb[0]) + pa[0].mul(pb[1])).mul(divisor));

		cv::Mat res;
		cv::merge(pres, res);
		return res;

	}

	cv::Mat complexDivisionreal(cv::Mat a, cv::Mat b)
	{
		std::vector<cv::Mat> pa;
		
		cv::split(a, pa);

		pa[0] = pa[0] / b;
		pa[1] = pa[1] / b;

		cv::Mat res;
		cv::merge(pa, res);
		return res;
	}



	cv::Mat complexbsxfun(cv::Mat a, cv::Mat b)
	{

		cv::Mat res(a.rows, a.cols, CV_32FC2);
		for (size_t i = 0; i < a.rows; i++)
		{
			for (size_t j = 0; j < a.cols; j++)
			{
				cv::Complex<float> ai(a.at<cv::Vec<float, 2> >(i, j)[0], a.at<cv::Vec<float, 2> >(i, j)[1]),
					bi(b.at<cv::Vec<float, 2> >(0, j)[0], b.at<cv::Vec<float, 2> >(0, j)[1]),c;
				c = ai * bi;
				res.at<cv::Vec<float, 2> >(i, j) = cv::Vec<float, 2>(c.re, c.im);
			}
		}

		/*
		std::vector<cv::Mat> pa;
		std::vector<cv::Mat> pb;
		cv::split(a, pa);
		cv::split(b, pb);

		std::vector<cv::Mat> pres;

		pres.push_back(pa[0].mul(pb[0]) - pa[1].mul(pb[1]));
		pres.push_back(pa[0].mul(pb[1]) + pa[1].mul(pb[0]));

		cv::Mat res;
		cv::merge(pres, res);
*/

		return res;
	}


	cv::Mat eng(cv::Mat img)
	{	
		cv::Mat res;
		std::vector<cv::Mat> planes;
		cv::split(img, planes); // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
		res = planes[0].mul(planes[0]) + planes[1].mul(planes[1]);
		return res;

	}

	void rearrange(const cv::Mat &org_img)
	{
		//using namespace cv;
		cv::Mat img;
		org_img.copyTo(img);

		// img = img(cv::Rect(0, 0, img.cols & -2, img.rows & -2));
		int cx = img.cols / 2;
		int cy = img.rows / 2;

		cv::Mat q0(img, cv::Rect(0, 0, cx, cy)); // Top-Left - Create a ROI per quadrant
		cv::Mat q1(img, cv::Rect(cx, 0, cx, cy)); // Top-Right
		cv::Mat q2(img, cv::Rect(0, cy, cx, cy)); // Bottom-Left
		cv::Mat q3(img, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

		cv::Mat tmp; // swap quadrants (Top-Left with Bottom-Right)
		q0.copyTo(tmp);
		q3.copyTo(q0);
		tmp.copyTo(q3);
		q1.copyTo(tmp); // swap quadrant (Top-Right with Bottom-Left)
		q2.copyTo(q1);
		tmp.copyTo(q2);
	}

	cv::Mat fftshift(const cv::Mat& org_img, bool rowshift, bool colshift, bool reverse)
	{
		cv::Mat temp(org_img.size(), org_img.type());
		//org_img.copyTo(temp);
		if (org_img.empty())
			return cv::Mat();

		int w = org_img.cols, h = org_img.rows;
		int rshift = reverse ? h - h / 2 : h/2,
			cshift = reverse ? w - w / 2 : w/2;

		for (int i = 0; i < org_img.rows; i++)
		{
			int ii = rowshift ? (i + rshift) % h : i;
			for (int j = 0; j < org_img.cols; j++)
			{
				int jj = colshift ? (j + cshift) % w : j;
				if (org_img.channels() == 2)
					temp.at<cv::Vec<float, 2> >(ii, jj) = org_img.at<cv::Vec<float, 2> >(i, j);
			 	else
					temp.at<float>(ii, jj) = org_img.at<float>(i, j);
			}
		}
		return temp;
	}





	cv::Mat mat_conj(const cv::Mat org)
	{
		//cv::Mat conj(org.size(), org.type());
		//if (org.empty())
		//	return org;
		//for (size_t r = 0; r < org.rows; r++)
		//{
		//	for (size_t  c = 0; c < org.cols; c++)
		//	{
		//		conj.at<cv::Vec<float, 2> >(r, c) = conjugate(org.at<cv::Vec<float, 2> >(r, c));
		//	}
		//}
		//return conj;
		if (org.empty())
			return org;
		std::vector<cv::Mat_<float> > planes;
		cv::split(org,planes);
		planes[1]=-planes[1];
		cv::Mat result;
		cv::merge(planes,result);
		return result;
		
		
	}

	float   mat_sum( cv::Mat org)
	{
		if (org.empty())
			return 0;

		float sum = 0;

		for (size_t r = 0; r < org.rows; r++)
		{
			float* orgPtr = org.ptr<float>(r);
			for (size_t c = 0; c < org.cols; c++)
			{
				//sum += org.at < float >(r, c);
				sum += orgPtr[c];
			}
		}

		return sum;
	
	}

	float   mat_sum( cv::Mat& org)
	{
		if (org.empty())
			return 0;

		float sum = 0;

		for (size_t r = 0; r < org.rows; r++)
		{
			float* orgPtr = org.ptr<float>(r);
			for (size_t c = 0; c < org.cols; c++)
			{
				//sum += org.at < float >(r, c);
				sum += orgPtr[c];
			}
		}

		return sum;
	
	}


	cv::Mat mat_conj(const cv::Mat& org)
	{
		//cv::Mat conj(org.size(), org.type());
		//if (org.empty())
		//	return org;
		//for (size_t r = 0; r < org.rows; r++)
		//{
		//	for (size_t  c = 0; c < org.cols; c++)
		//	{
		//		conj.at<cv::Vec<float, 2> >(r, c) = conjugate(org.at<cv::Vec<float, 2> >(r, c));
		//	}
		//}
		//return conj;
		if (org.empty())
			return org;
		std::vector<cv::Mat_<float> > planes;
		cv::split(org,planes);
		planes[1]=-planes[1];
		cv::Mat result;
		cv::merge(planes,result);
		return result;
		
		
	}



	void normalizedLogTransform(cv::Mat &img)
	{
		img = cv::abs(img);
		img += cv::Scalar::all(1);
		cv::log(img, img);
		// cv::normalize(img, img, 0, 1, CV_MINMAX);
	}


	cv::Mat cmat_multi(const cv::Mat&a, const cv::Mat& b)
	{
		if (a.empty() || b.empty())
			return a;

		if (a.cols != b.rows)
			assert("Unmatched Size");

		cv::Mat res(a.rows, b.cols, CV_32FC2);
		for (size_t i = 0; i < res.rows; i++)
		{
			for (size_t j = 0; j < res.cols; j++)
			{
				cv::Complex<float> rest(0, 0);
				for (size_t k = 0; k < a.cols; k++)
				{
					rest += cv::Complex<float>(a.at<cv::Vec<float, 2> >(i, k)[0], a.at<cv::Vec<float, 2> >(i, k)[1])
						* cv::Complex<float>(b.at<cv::Vec<float, 2> >(k, j)[0], b.at<cv::Vec<float, 2> >(k, j)[1]);
				}
				res.at<cv::Vec<float, 2> >(i, j) = cv::Vec<float, 2>(rest.re, rest.im);
			}
		}

		return res;
	}

	cv::Mat real2complx(const cv::Mat& x)
	{
		if (x.empty() || x.channels() == 2)
			return x;

		std::vector<cv::Mat>  c ;
		c.push_back(x);
		c.push_back(cv::Mat::zeros(x.size(), CV_32FC1));
		cv::Mat res;
		cv::merge(c, res);
		return res;
	}

	cv::Mat  conv_complex(cv::Mat _a, cv::Mat _b, bool valid)

	{
		_a = real2complx(_a);
		_b = real2complx(_b);

		//*** preprocess ****
		cv::Mat a = cv::Mat::zeros(_a.rows + _b.rows - 1, _a.cols + _b.cols - 1, CV_32FC2);  // padding with zeros
		cv::Point pos(_b.cols / 2, _b.rows / 2);

		_a.copyTo(a(cv::Rect(_b.cols - 1 - pos.x, _b.rows - 1 - pos.y, _a.cols, _a.rows)));

		cv::Mat b = _b.clone();
		rot90(b, 3);
		cv::Mat res;

		if (a.channels() != 2 || b.channels() != 2)
			return res;
		std::vector<cv::Mat> va, vb;
		cv::split(a, va);
		cv::split(b, vb);

		cv::Mat r, i, r1, r2, i1, i2;

		cv::filter2D(va[0], r1, -1, vb[0], cv::Point(-1, -1), 0, cv::BORDER_ISOLATED);
		cv::filter2D(va[1], r2, -1, vb[1], cv::Point(-1, -1), 0, cv::BORDER_ISOLATED);
		cv::filter2D(va[0], i1, -1, vb[1], cv::Point(-1, -1), 0, cv::BORDER_ISOLATED);
		cv::filter2D(va[1], i2, -1, vb[0], cv::Point(-1, -1), 0, cv::BORDER_ISOLATED);

		r = r1 - r2;
		i = i1 + i2;
		std::vector<cv::Mat> ri ;
		ri.push_back(r);
		ri.push_back(i);
		cv::merge(ri, res);
		if (valid)
		{
			return res(cv::Rect(_b.cols - 1, _b.rows - 1, _a.cols - _b.cols + 1, _a.rows - _b.rows + 1));
		}
		else
		{
			return res;
		}
	}



}


