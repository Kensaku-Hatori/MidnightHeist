//****************************************************************
//
// マップオブジェクト更新システムの処理[UpdateMapobjectSystem.h]
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
#include "SizeComponent.hpp"

// モデルの更新処理
struct UpdateMapobjectSystem : BaceSystem
{
	// コンストラクタ
	UpdateMapobjectSystem() = default;
	// デストラクタ
	~UpdateMapobjectSystem() = default;
	// 更新処理
	void Update(entt::registry& reg)override;
	// ゴールとの当たり判定
	void CollisionExitGate(entt::registry& Reg, entt::entity MapObject);
};