#pragma once

#include "Common.h"

namespace Image
{
	using namespace Common;
	struct CResolution
	{
		CResolution() = default;
		CResolution(int W, int H) : W(W), H(H) {}
		int W = 0, H = 0;
	};

	struct CContours;
	struct CGrayScaleImage;

	using CNativeImage = cv::Mat;
	using CRect = cv::Rect;

	struct CImage
	{
		CImage(CNativeImage Img) : m_Image(move(Img)) {}
		CResolution Resolution() const
		{
			return { m_Image.rows, m_Image.cols };
		}

		CRect Rect() const
		{
			return { 0, 0, m_Image.rows, m_Image.cols };
		}

		CNativeImage& get() const
		{
			return m_Image;
		}
	protected:
		mutable CNativeImage m_Image;
	};

	struct CRGBImage : CImage
	{
		CRGBImage() = default;
		CRGBImage(CNativeImage m) : CImage(move(m)) {}
	};

	inline CRGBImage LoadImage(const string &FileName)
	{
		return CRGBImage{ cv::imread(FileName) };
	}

	struct CGrayScaleImage : CImage
	{
		CGrayScaleImage() = default;
		CGrayScaleImage(CResolution r) :
			CImage(cv::Mat{r.W, r.H, cv::DataType<unsigned char>::type})
		{
			
		}
	};

	inline CGrayScaleImage Convert(const CRGBImage &r)
	{
		CGrayScaleImage i{ r.Resolution() };
		cv::cvtColor(r.get(), i.get(), cv::COLOR_RGB2GRAY);
		return i;
	}

	inline void Threshold(CGrayScaleImage& Img, double threshold, double maxval)
	{
		cv::threshold(Img.get(), Img.get(), threshold, maxval, cv::THRESH_BINARY);
	}



}
