//================================================================
//
// タイトルマネージャーの更新処理[UpdateTitleSystem.h]
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
// タイトルマネージャーのアップデート
//****************************************************************
struct UpdateTitleManagerSystem final : BaseSystem
{
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="Reg"></param>
	void Update(entt::registry& Reg)override;
private:
	/// <summary>
	/// タイトルメニューの更新
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Manager">マネージャー</param>
	void UpdateTitleMenu(entt::registry& Reg,entt::entity& Manager);
	/// <summary>
	/// メニューが選ばれた時の処理
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Menu">選ばれたメニュー</param>
	void FunctionMenu(entt::registry& Reg, entt::entity& Menu);
};