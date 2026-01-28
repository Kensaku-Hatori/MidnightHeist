//****************************************************************
//
// 物理エンジン用のカプセルの更新システムの処理[UpdatebtCapsuleColliderSystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// モデルの更新処理
struct UpdatebtCapsuleColliderSystem : BaceSystem
{
	// コンストラクタ
	UpdatebtCapsuleColliderSystem() = default;
	// デストラクタ
	~UpdatebtCapsuleColliderSystem() = default;
	// 更新処理
	void Update(entt::registry& reg)override;
};