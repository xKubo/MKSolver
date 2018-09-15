// MKSolver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Contour.h"
#include "GUI.h"

#include <iostream>
#include "Stats.h"

struct CMKSolver
{

};

int main()
{
	try
	{
		using std::cout;
		using std::endl;
		GUI::CWindow Orig{ "Orig" }, Thr{ "Thr" }, Cont{ "Cont" };
		auto im = Image::LoadImage("input/1.png");
		CMKSolver s;
		//s.Solve(im);
		Image::CGrayScaleImage imGray = Image::Convert(im);
		Threshold(imGray, 160, 255);
		//cv::threshold(imGray.get(), imGray.get(), 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
		auto Contours = FindContours(imGray);
		cv::Mat x = im.get().clone();
		Image::CRGBImage im2{ x };
		auto hist = Stats::MakeHistogram<Image::CContour>([](const Image::CContour& c) {return static_cast<int>(c.Area()); });
		int Pixels90 = imGray.Resolution().W*imGray.Resolution().H * 9 / 10;  // 
		int MinArea = Pixels90 / (50 * 50);
		int MaxArea = Pixels90 / (20 * 20);
		for (int i = 0; i < Contours.Count(); ++i)
			if (!Contours.HasHole(i))
			{
				auto ct = Contours.Contour(i);
				hist.Add(ct);
				if (ct.Area() > MinArea && ct.Area() < MaxArea)
				{

					DrawContour(im, Contours, i);
				}
			}

		Stats::CHistogramGraph w{ Image::CResolution{300, 300} };
		w.Update(hist, MinArea, MaxArea);
		GUI::CWindow Hist("hist");

		Show(w.Image(), Hist);
		Show(im2, Orig);
		Show(im, Cont);
		Show(imGray, Thr);

		GUI::WaitForKey();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error : " << e.what() << std::endl;
		return 1;
	}


    return 0;
}

