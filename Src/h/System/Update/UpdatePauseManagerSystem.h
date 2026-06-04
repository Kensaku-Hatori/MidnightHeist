//================================================================
//
// ポーズマネージャーの更新処理[UpdatePauseSystem.h]
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
// ポーズマネージャーの更新処理
//****************************************************************
struct UpdatePauseManagerSystem final : BaseSystem
{
	/// <summary>
	/// コンストラクタ(ポーズを参照しない)
	/// </summary>
	UpdatePauseManagerSystem() : BaseSystem(false) {};
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	void Update(entt::registry& Reg)override;
private:
	/// <summary>
	/// ポーズメニューの更新
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Manager">マネージャー</param>
	void UpdatePauseMenu(entt::registry& Reg, entt::entity& Manager);
	/// <summary>
	/// メニューが選ばれた時
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Menu">選ばれたメニュー</param>
	void FunctionMenu(entt::registry& Reg, entt::entity& Menu);
};