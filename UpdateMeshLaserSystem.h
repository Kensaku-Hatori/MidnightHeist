//****************************************************************
//
// メッシュレーザーの更新処理[UpdateMeshLaser.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

struct UpdateMeshLaserSystem : BaceSystem 
{
	// コンストラクタ
	UpdateMeshLaserSystem() = default;
	// デストラクタ
	~UpdateMeshLaserSystem() = default;
	// 更新
	void Update(entt::registry& Reg)override;
	void UpdateVertex(entt::registry& Reg, entt::entity Entity);

	// マップオブジェクトとの当たり判定
	bool CollisionEntity(entt::registry& Reg,entt::entity Entity,entt::entity ToEntity,float& Distance);
};