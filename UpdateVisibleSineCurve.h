//****************************************************************
//
// サインカーブの更新システムの処理[UpdateVisibleSineCurveSystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// モデルの更新処理
struct UpdateVisibleSineCurveSystem : BaceSystem
{
	// コンストラクタ
	UpdateVisibleSineCurveSystem() = default;
	// デストラクタ
	~UpdateVisibleSineCurveSystem() = default;
	// 更新処理
	void Update(entt::registry& reg)override;
};