//****************************************************************
//
// 物理エンジン用の矩形の更新システムの処理[UpdatebtBoxColliderSystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// モデルの更新処理
struct UpdatebtBoxColliderSystem : BaceSystem
{
	// コンストラクタ
	UpdatebtBoxColliderSystem() = default;
	// デストラクタ
	~UpdatebtBoxColliderSystem() = default;
	// 更新処理
	void Update(entt::registry& reg)override;
};