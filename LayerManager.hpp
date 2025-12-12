//****************************************************************
//
// レイヤー順にソートされたエンテティのリスト取得[LayerManager.hpp]
// Author Kensaku Hatori
//
//****************************************************************

#pragma once

#include "LayerComponent.hpp"

namespace LayerManager
{
	template<typename...Comps>
	std::vector<entt::entity> GetSortEntityList(entt::registry& reg)
	{
		// 型が入っていなかったらコンパイルエラーを出す
		static_assert(sizeof...(Comps) > 0, "<<<<<< type not set >>>>>>>>");

		std::vector<entt::entity> EntiryList;
		auto view = reg.view<LayerComp, Comps...>();
		EntiryList.reserve(view.size_hint());

		for (auto Entity : view)
		{
			if (reg.valid(Entity) == false) continue;
			EntiryList.push_back(Entity);
		}

		// レイヤー順でソート
		std::sort(EntiryList.begin(), EntiryList.end(),
			[&](entt::entity EntityA, entt::entity EntityB)
			{
				return reg.get<LayerComp>(EntityA).nLayer < reg.get<LayerComp>(EntityB).nLayer;
			});

		return EntiryList;
	}
}