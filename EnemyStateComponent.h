//****************************************************************
//
// 敵の状態の情報[EnemyStateComponentonent.hpp]
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

// 敵のステートコンポーネント
struct EnemyStateComponent {
	// コンストラクタ
	EnemyStateComponent(EnemyState::ENEMYSTATE Default) : State(Default) {};
	// 管理者
	EnemyState::ENEMYSTATE State;
};