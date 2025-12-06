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
struct UpdateEnemyPredictSystem : BaceSystem
{
	// コンストラクタ・デストラクタ
	UpdateEnemyPredictSystem() = default;
	~UpdateEnemyPredictSystem() = default;
	// 更新
	void Update(entt::registry& reg)override;
};