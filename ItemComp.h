//****************************************************************
//
// アイテムのコンポーネント[ItemComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 念のため
namespace ItemConfig {
	// 解錠にかかるフレーム数
	static constexpr int nFramePicking = 300;
	// 一フレーム解錠ゲージの増加量
	static constexpr float Ratio = 1.0f / nFramePicking;
}
// アイテムの情報
struct ItemComp {
	// コンストラクタ
	ItemComp(const float _InteractSize = NULL) :
		InteractSize(_InteractSize) {};
	// 解錠の進行度
	int nCntPicking;
	// アイテムの大きさ
	float InteractSize;
};