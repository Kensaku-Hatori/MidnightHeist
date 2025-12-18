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
#include "RigitBodyComponent.hpp"
#include "SingleCollisionShapeComponent.hpp"
#include "CapsuleComponent.hpp"

// プレイヤーのアップデート
struct UpdateTitlePlayerSystem : BaceSystem
{
	// コンストラクタ・デストラクタ
	UpdateTitlePlayerSystem() = default;
	~UpdateTitlePlayerSystem() = default;
	// 描画
	void Update(entt::registry& reg)override;
	void UpdateRB(entt::registry& reg, entt::entity Player);
};