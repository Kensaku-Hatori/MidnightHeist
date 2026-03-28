//****************************************************************
//
// アイテム管理のコンポーネント[ItemManagerComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// アイテムの情報
struct ItemManagerComponent {
	// コンストラクタ
	ItemManagerComponent(std::vector<entt::entity>& _ItemList) :
		NowAnimIdx(0), ItemLiset(_ItemList), IsFinished(false) {};
	// アイテム管理に必要な情報
	int NowAnimIdx;
	std::vector<entt::entity> ItemLiset;
	bool IsFinished;
};