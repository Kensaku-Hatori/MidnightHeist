//================================================================
//
// 敵の帰る更新処理[UpdateEnemyBackSystem.h]
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
// 敵帰るのアップデート
//****************************************************************
struct UpdateEnemyBackSystem final : BaseSystem
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
	/// <param name="Entit">敵</param>
	void UpdateMove(entt::registry& Reg, entt::entity& Entit);
};