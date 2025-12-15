//****************************************************************
//
// アイテムのコンポーネント[ItemComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// アイテムの情報
struct ItemComp {
	// コンストラクタ
	ItemComp(const float _InteractSize = NULL) :
		InteractSize(_InteractSize) {};
	// アイテムの大きさ
	float InteractSize;
};