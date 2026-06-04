//================================================================
//
// 敵の巡回更新処理[UpdateEnemySearchSystem.h]
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
// 敵のアップデート
//****************************************************************
struct UpdateEnemySearchSystem final : BaseSystem
{
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	void Update(entt::registry& Reg)override;
private:
	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Entity">敵</param>
	void UpdateMove(entt::registry& Reg, entt::entity Entity);
};