#pragma once

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


	private:
		vector<TObject> m_Objects;
		FPred m_Pred;
		FGetKey m_Key;
	};
}