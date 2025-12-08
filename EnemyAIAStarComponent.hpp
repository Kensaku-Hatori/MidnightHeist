//****************************************************************
//
// 敵AIの最短経路検索用の情報[EnemyAIAStarComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "EnemyPatrolPointComp.hpp"

namespace EnemyAI {
	struct AStarPointInfo
	{
		PatrolPoint::PatrolPointInfo PatrolPoint;
		float Cost;
		int MyIdx;
	};
}
// 敵のステートコンポーネント
struct EnemtAIAstarComp {
	// コンストラクタ
	EnemtAIAstarComp() = default;
	// 計算結果のルート
	std::vector<int> DestRoute;
};