//****************************************************************
//
// 2D更新システムの処理[2DUpdatesystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// ２D更新処理を定義
struct Update2DSystem : BaceSystem
{
	// コンストラクタ
	Update2DSystem() = default;
	// デストラクタ
	~Update2DSystem() = default;
	// 更新
	void Update(entt::registry& Reg)override;
};