#pragma once

#include "Common.h"
#include "Image.h"

namespace GUI
{
	using namespace Common;

	struct CWindow
	{
		CWindow(const string& Name) : 
			m_Name(Name)
		{

		}

		string Name() const
		{
			return m_Name;
		}
	private:
		string m_Name;
	};

	void Show(const Image::CImage& i, CWindow& w)
	{
		cv::imshow(w.Name(), i.get());
	}

	void WaitForKey()
	{
		cv::waitKey();
	}

}
