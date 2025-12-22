//****************************************************************
//
// マトリックスの計算処理[UpdateMtxsystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// ２D更新処理を定義
struct UpdateMtxSystem : BaceSystem
{
	// コンストラクタ
	UpdateMtxSystem() = default;
	// デストラクタ
	~UpdateMtxSystem() = default;
	// 更新
	void Update(entt::registry& Reg)override;
};