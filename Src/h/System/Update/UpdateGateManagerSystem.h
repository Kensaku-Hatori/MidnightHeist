//****************************************************************
//
// ゲートマネージャーシステムの処理[UpdateGateManager.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// ゲートマネージャーの更新処理
struct UpdateGateManagerSystem : BaceSystem
{
	// コンストラクタ
	UpdateGateManagerSystem() = default;
	// デストラクタ
	~UpdateGateManagerSystem() = default;
	// 更新処理
	void Update(entt::registry& reg)override;
};