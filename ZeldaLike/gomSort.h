#pragma once

namespace gomSort
{
	struct SortKey
	{
		unsigned char layer;
		float yPosPlusWidth;
	};

	struct SortCompare
	{
		//TODO: Bulletproof
		bool operator()(const SortKey& lhs, const SortKey& rhs) const
		{
			if (lhs.layer < rhs.layer)
				return lhs.yPosPlusWidth < rhs.yPosPlusWidth;
			else
				return false;
		}
	};
}