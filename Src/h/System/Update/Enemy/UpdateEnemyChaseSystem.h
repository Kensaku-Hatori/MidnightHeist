//================================================================
//
// 敵の追跡更新処理[UpdateEnemyChaseSystem.h]
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
#include "System/Update/BaseUpdatesystem.hpp"

//****************************************************************
// 敵の追跡アップデート
//****************************************************************
struct UpdateEnemyChaseSystem final : BaseSystem
{
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	void Update(entt::registry& Reg)override;
private:
	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Entity">敵</param>
	/// <param name="PlayerEntity">プレイヤー</param>
	void UpdateMove(entt::registry& Reg, entt::entity& Entity, entt::entity& PlayerEntity);
};