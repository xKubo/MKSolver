#pragma once
#include "Image.h"

namespace Image
{
	using TContData = vector<vector<cv::Point>>;
	using TContHierarchy = std::vector<cv::Vec4i>;

	struct CContour
	{
		CContour(vector<cv::Point> cd) : m_Contour(cd) 
		{
			m_Area = cv::contourArea(m_Contour);
		}
		CRect BoundingBox() const
		{
			return cv::boundingRect(m_Contour);
		}

		auto Area() const
		{
			return m_Area;
		}
	private:
		double m_Area;
		vector<cv::Point> m_Contour;
	};

	struct CContours
	{
		CContours(TContData cd, TContHierarchy h) : m_Contours(cd), m_Hierarchy(h) {}
		const TContData& get() const
		{
			return m_Contours;
		}

		CRect BoundingBox(int Idx) const
		{
			return cv::boundingRect(m_Contours[Idx]);
		}

		CContour Contour(int Idx) const
		{
			return m_Contours[Idx];
		}

		bool HasHole(int Idx) const
		{
			return m_Hierarchy[Idx][2] >= 0;
		}

		int Count() const
		{
			return m_Contours.size();
		}
	private:
		vector<vector<cv::Point>> m_Contours;
		std::vector<cv::Vec4i> m_Hierarchy;
	};

	CContours FindContours(CGrayScaleImage &Img)
	{
		TContData res;
		TContHierarchy h;
		cv::findContours(Img.get(), res, h,cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
		return { res, h };
	}

	void DrawContour(CImage &i, const CContours& c, int ContourIdx)
	{
		cv::drawContours(i.get(), c.get(), ContourIdx, cv::Scalar{ 255, 0, 0 });
	}


}