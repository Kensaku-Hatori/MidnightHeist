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
struct PlayerUpdateSystem : BaceSystem
{
	// コンストラクタ・デストラクタ
	PlayerUpdateSystem() = default;
	~PlayerUpdateSystem() = default;
	// 描画
	void Update(entt::registry& reg)override;
	void UpdateRB(Transform3D& TransformCmp, RigitBodyComp& RBCmp,SingleCollisionShapeComp& ColliderCmp,CapsuleComp& CapsuleCmp);
	void UpdateMovement(Transform3D& TransformCmp,RigitBodyComp& RBCmp);
	void UpdateLockOn(entt::registry& reg, entt::entity Player);
};