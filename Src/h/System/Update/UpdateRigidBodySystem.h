//****************************************************************
//
// 剛体の更新システムの処理[UpdateRigitBodySystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// モデルの更新処理
struct UpdateRigidBodySystem : BaceSystem
{
public:
	// コンストラクタ
	UpdateRigidBodySystem();
	// デストラクタ
	~UpdateRigidBodySystem() = default;
	// 更新処理
	void Update(entt::registry& reg)override;
private:
	void OnRigidBodyComponentConstruct(entt::registry& Reg, entt::entity Entity);
	void OnRigidBodyComponentDestruct(entt::registry& Reg, entt::entity Entity);
};