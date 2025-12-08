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

// 敵のアップデート
struct UpdateEnemyChaseSystem : BaceSystem
{
	// コンストラクタ・デストラクタ
	UpdateEnemyChaseSystem() = default;
	~UpdateEnemyChaseSystem() = default;
	// 更新
	void Update(entt::registry& reg)override;
	// 移動の更新
	void UpdateMove(entt::registry& Reg, entt::entity& Entity, entt::entity& PlayerEntity);
};