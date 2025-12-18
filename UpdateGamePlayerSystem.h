//****************************************************************
//
// プレイヤー更新システムの処理[playerUpdateSystem.h]
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
struct UpdateGamePlayerSystem : BaceSystem
{
	// コンストラクタ・デストラクタ
	UpdateGamePlayerSystem() = default;
	~UpdateGamePlayerSystem() = default;
	// 描画
	void Update(entt::registry& reg)override;
	void UpdateRB(entt::registry& reg, entt::entity Player);
	void UpdateMovement(entt::registry& reg, entt::entity Player);
	void UpdateLockOn(entt::registry& reg, entt::entity Player);
	void UpdateUnLock(entt::registry& Reg, entt::entity Player);
	void UpdateState(entt::registry& Reg, entt::entity Player);
};