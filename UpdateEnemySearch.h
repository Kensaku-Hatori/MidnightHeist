//****************************************************************
//
// 敵の追跡更新処理[UpdateEnemySearchSystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// 敵のアップデート
struct UpdateEnemySearchSystem : BaceSystem
{
	// コンストラクタ・デストラクタ
	UpdateEnemySearchSystem() = default;
	~UpdateEnemySearchSystem() = default;
	// 更新
	void Update(entt::registry& reg)override;
	void UpdateAStar(entt::registry& Reg, entt::entity Entity);
	// 移動の更新
	void UpdateMove(entt::registry& Reg, entt::entity Entity);
};