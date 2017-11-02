// MKSolver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Contour.h"
#include "GUI.h"

#include <iostream>

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
		Image::CGrayScaleImage imGray = Image::Convert(im);
		Threshold(imGray, 160, 255);
		auto Contours = FindContours(imGray);
		cv::Mat x = im.get().clone();
		Image::CRGBImage im2{ x };
		for (int i = 0; i < Contours.Count(); ++i)
			if (!Contours.HasHole(i))
				DrawContour(im, Contours, i);


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

