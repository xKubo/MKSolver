#pragma once

#include "GUI.h"
#include "Image.h"

namespace Stats
{
	
	template <typename T, typename FPred, typename FGetKey >
	struct CHistogram
	{
		using TObject = T;
		using TKey = decltype(FGetKey(std::declval(T)));
		CHistogram() = default;
		
		CHistogram(FPred p) : m_Pred(p)
		{

		}

		template <typename TRange>
		void Add(const TRange& r)
		{
			auto it = m_Objects.insert(m_Objects.end(), r.begin(), r.end());
			inplace_merge(m_Objects.begin(), it, m_Objects.end(), m_Pred);
		}

		const vector<TObject>& Objects() const
		{
			return m_Objects;
		}

		TKey GetKey(const TObject& o)
		{
			return m_Key(o);
		}


	private:
		vector<TObject> m_Objects;
		FPred m_Pred;
		FGetKey m_Key;
	};

	struct CHistogramWindow
	{
		CHistogramWindow(GUI::CWindow Window, Image::CResolution HistRes) : 
			m_Window(Window),
			m_Res(HistRes),
			m_Image(HistRes),
			m_Counts(m_Res.W)
		{
			
		}

		template <typename T, typename FPred, typename FGetKey >
		void Update(const CHistogram<T, FPred, FGetKey>& h)
		{
			const auto& os = h.Objects();
			
			fill(m_Counts.begin(), m_Counts.end(), 0);
			for (int i = 0; i < os.size(); ++i)
			{
				int k = h.GetKey(o);
				m_Counts[k]++;
				
			}

			cv::rectangle(m_Image, m_Image.Rect(), 0, CV_FILLED);
			for (int i=0; i<m_Image.get().rows; ++i)
				cv::line(m_Image, Point{i, 0}, Point{i, m_Counts[i] }, 255);

		}
	private:

		vector<int> m_Counts;
		Image::CResolution m_Res;
		Image::CRGBImage m_Image;
		GUI::CWindow m_Window;
	};
}