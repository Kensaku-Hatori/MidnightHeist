//****************************************************************
//
// 敵の追跡更新処理[UpdateEnemyChaseSystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// 敵前に予測のアップデート
struct UpdateEnemyPredictFrontSystem : BaceSystem
{
	// コンストラクタ・デストラクタ
	UpdateEnemyPredictFrontSystem() = default;
	~UpdateEnemyPredictFrontSystem() = default;
	// 更新
	void Update(entt::registry& reg)override;
};

// 敵のぶつかって沿う感じアップデート
struct UpdateEnemyPredictFollowSystem : BaceSystem
{
	// コンストラクタ・デストラクタ
	UpdateEnemyPredictFollowSystem() = default;
	~UpdateEnemyPredictFollowSystem() = default;
	// 更新
	void Update(entt::registry& reg)override;
};