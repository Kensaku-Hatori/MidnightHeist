//****************************************************************
//
// タイトルプレイヤー更新システムの処理[UpdateTitlePlayerSystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "System/Update/baceUpdatesystem.hpp"
#include "Component/TransformComponent.hpp"

// プレイヤーのアップデート
struct UpdateTitlePlayerSystem : BaceSystem
{
	// コンストラクタ・デストラクタ
	UpdateTitlePlayerSystem() = default;
	~UpdateTitlePlayerSystem() = default;
	// 描画
	void Update(entt::registry& reg)override;
};