//****************************************************************
//
// メッシュフィールド更新システムの処理[UpdateMeshFieldsystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// ２D更新処理を定義
struct UpdateMeshFieldSystem : BaceSystem
{
	// コンストラクタ
	UpdateMeshFieldSystem() = default;
	// デストラクタ
	~UpdateMeshFieldSystem() = default;
	// 更新
	void Update(entt::registry& Reg)override;
};