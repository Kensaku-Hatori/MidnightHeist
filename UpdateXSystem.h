//****************************************************************
//
// モデル更新システムの処理[3DUpdatesystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// モデルの更新処理
struct UpdateXSystem : BaceSystem
{
	// コンストラクタ
	UpdateXSystem() = default;
	// デストラクタ
	~UpdateXSystem() = default;
	// 更新処理
	void Update(entt::registry& reg)override;
};