//****************************************************************
//
// 敵の帰る更新処理[UpdateEnemyBackSystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// 敵のアップデート
struct UpdateEnemyBackSystem : BaceSystem
{
	// コンストラクタ・デストラクタ
	UpdateEnemyBackSystem() = default;
	~UpdateEnemyBackSystem() = default;
	// 更新
	void Update(entt::registry& reg)override;
	// 移動の更新
	void UpdateMove(entt::registry& Reg, entt::entity& Entit);
};