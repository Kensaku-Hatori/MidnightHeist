//****************************************************************
//
// 敵の更新処理[UpdateEnemySystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// 敵のアップデート
struct UpdateEnemySystem : BaceSystem
{
	// コンストラクタ・デストラクタ
	UpdateEnemySystem() = default;
	~UpdateEnemySystem() = default;
	// 更新
	void Update(entt::registry& reg)override;
	void UpdateMovement(entt::registry& Reg, entt::entity& Entity);
	void UpdateRB(entt::registry& Reg, entt::entity& Entity);
};