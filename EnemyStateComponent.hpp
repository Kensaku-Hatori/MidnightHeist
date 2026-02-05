//****************************************************************
//
// 敵のステートの情報[EnemyStateComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 念のため
namespace EnemyState {
	// ステートの列挙型
	enum class ENEMYSTATE
	{
		NONE = 0,
		CHASE,
		PREDICT,
		SEARCH,
		BACK,
		MAX
	};
}

// ステートの情報
struct EnemyStateComponent {
	// 管理者
	EnemyState::ENEMYSTATE State;
};