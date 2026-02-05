//****************************************************************
//
// 敵AIの最短経路検索用の情報[EnemyAIAStarComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// A*コンポーネント
struct AstarComponent {
	// コンストラクタ
	AstarComponent() = default;
	// 計算結果のルート
	std::vector<int> DestRoute;
};