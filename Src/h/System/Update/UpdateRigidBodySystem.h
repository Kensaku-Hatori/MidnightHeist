//================================================================
//
// 剛体の更新システムの処理[UpdateRigitBodySystem.h]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// 二重インクルード防止
//****************************************************************
#pragma once

//****************************************************************
// インクルード
//****************************************************************
#include "BaseUpdatesystem.hpp"

//****************************************************************
// 剛体の更新処理
//****************************************************************
struct UpdateRigidBodySystem final : BaseSystem
{
	UpdateRigidBodySystem();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	void Update(entt::registry& Reg)override;
private:
	/// <summary>
	/// 剛体が追加された時
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Entity">エンティティ</param>
	void OnRigidBodyComponentConstruct(entt::registry& Reg, entt::entity Entity);
	/// <summary>
	/// 剛体が削除された時
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Entity">エンティティ</param>
	void OnRigidBodyComponentDestruct(entt::registry& Reg, entt::entity Entity);
};