//****************************************************************
//
// アイテム更新システムの処理[UpdateItemSystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// モデルの更新処理
struct UpdateItemSystem : BaceSystem
{
	// コンストラクタ
	UpdateItemSystem() = default;
	// デストラクタ
	~UpdateItemSystem() = default;
	// 更新処理
	void Update(entt::registry& reg)override;
	void UpdateLockOn(entt::registry& Reg, entt::entity& Entity);
};