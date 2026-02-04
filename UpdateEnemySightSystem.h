//****************************************************************
//
// 敵の視界更新システムの処理[UpdateEnemySightSystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// プレイヤーのアップデート
struct UpdateEnemySightSystem : BaceSystem
{
	// コンストラクタ・デストラクタ
	UpdateEnemySightSystem() = default;
	~UpdateEnemySightSystem() = default;
	// 更新
	void Update(entt::registry& reg)override;
};