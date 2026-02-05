//****************************************************************
//
// タイトルプレイヤー更新システムの処理[UpdateTitlePlayerSystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"
#include "TransformComponent.hpp"
#include "CapsuleComponent.hpp"

// プレイヤーのアップデート
struct UpdateTitlePlayerSystem : BaceSystem
{
	// コンストラクタ・デストラクタ
	UpdateTitlePlayerSystem() = default;
	~UpdateTitlePlayerSystem() = default;
	// 描画
	void Update(entt::registry& reg)override;
};