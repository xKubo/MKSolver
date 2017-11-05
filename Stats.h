#pragma once

#include "GUI.h"
#include "Image.h"

namespace Stats
{
	using namespace	Common;

	template <typename T, typename FPred, typename FGetKey >
	struct CHistogram
	{
		using TObject = T;
		//using TKey = decltype(FGetKey(std::declval<TObject>()));
		CHistogram() = default;
		
		CHistogram(FPred p, FGetKey k) : m_Pred(p), m_Key(k)
		{

		}

		void Add(const TObject& o)
		{
			auto it = lower_bound(m_Objects.begin(), m_Objects.end(), o, m_Pred);
			m_Objects.emplace(it, o);
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

		auto GetKey(const TObject& o) const
		{
			return m_Key(o);
		}


	private:
		vector<TObject> m_Objects;
		FPred m_Pred;
		FGetKey m_Key;
	};



	template <typename T, typename FPred, typename FGetKey>
	inline auto MakeHistogram(FGetKey k, FPred p)
	{
		return CHistogram<T, FPred, FGetKey>(p, k);
	}

	template <typename T, typename FGetKey>
	inline auto MakeHistogram(FGetKey k)
	{
		return MakeHistogram<T>(k, [k](const T& t1, const T& t2) { return k(t1) < k(t2); });
	}


	struct CHistogramGraph
	{
		CHistogramGraph(Image::CResolution HistRes) : 
			m_Image(HistRes),
			m_Counts(HistRes.W)
		{
			
		}

		template <typename T, typename FPred, typename FGetKey >
		void Update(const CHistogram<T, FPred, FGetKey>& h)
		{
			const auto& os = h.Objects();
			if (os.empty())
				return;
			vector<int> ObjectKeys;
			fill(m_Counts.begin(), m_Counts.end(), 0);
			for (int i = 0; i < os.size(); ++i)
			{
				ObjectKeys.push_back(h.GetKey(os[i]));				
			}

			int MaxVal = *std::max_element(ObjectKeys.begin(), ObjectKeys.end());
			int ValPerCol = MaxVal / m_Counts.size() + 1;

			assert(ValPerCol != 0);

			for (int k : ObjectKeys)
			{
				m_Counts[k / ValPerCol]++;
			}

			cv::rectangle(m_Image.get(), m_Image.Rect(), 0, CV_FILLED);
			for (int i=0; i<m_Image.get().rows; ++i)
				cv::line(m_Image.get(), cv::Point{ i, m_Image.Resolution().H }, cv::Point{ i, m_Image.Resolution().H - m_Counts[i] }, {0,255,0});
		}

		Image::CRGBImage& Image()
		{
			return m_Image;
		}
	private:
		vector<int> m_Counts;
		Image::CRGBImage m_Image;
	};
}