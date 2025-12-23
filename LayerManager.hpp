//****************************************************************
//
// レイヤー順にソートされたエンテティのリスト取得[LayerManager.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "LayerComponent.hpp"

// 念のため
namespace LayerManager
{
	// テンプレート可変長引数
	template<typename...Comps>
	std::vector<entt::entity> GetSortEntityList(entt::registry& reg)
	{
		// 型が入っていなかったらコンパイルエラーを出す
		static_assert(sizeof...(Comps) > 0, "<<<<<< type not set >>>>>>>>");

		// ソートした後の配列用変数
		std::vector<entt::entity> EntiryList;
		// レイヤーコンポーネントのビュー
		auto view = reg.view<LayerComp, Comps...>();
		// ビューの大きさ分サイズを確保
		EntiryList.reserve(view.size_hint());

		// 配列に情報を連結していく
		for (auto Entity : view)
		{
			// 無効だったら早期リターン
			if (reg.valid(Entity) == false) continue;
			// 連結
			EntiryList.push_back(Entity);
		}

		// レイヤー順でソート
		std::sort(EntiryList.begin(), EntiryList.end(),
			[&](entt::entity EntityA, entt::entity EntityB)
			{
				// 条件式
				return reg.get<LayerComp>(EntityA).nLayer < reg.get<LayerComp>(EntityB).nLayer;
			});

		// 情報を返す
		return EntiryList;
	}
}