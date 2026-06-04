//================================================================
//
// 敵の更新処理[UpdateEnemySystem.h]
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
struct UpdateEnemySystem final : BaseSystem
{
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	void Update(entt::registry& Reg)override;
private:
	/// <summary>
	/// 光線の更新
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Entity">敵</param>
	void UpdateRays(entt::registry& Reg, entt::entity& Entity);
	/// <summary>
	/// プレイヤーまでの光線を更新
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Entity">敵</param>
	void UpdateToPlayerRay(entt::registry& Reg, entt::entity& Entity);
};