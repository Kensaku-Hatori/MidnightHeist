//****************************************************************
//
// タイマー更新システムの処理[UpdateTimerSystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// タイマーの更新処理
struct UpdateTimerSystem : BaceSystem
{
	// コンストラクタ
	UpdateTimerSystem() = default;
	// デストラクタ
	~UpdateTimerSystem() = default;
	// 更新処理
	void Update(entt::registry& reg)override;
};