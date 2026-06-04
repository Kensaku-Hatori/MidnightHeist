//================================================================
//
// プレイヤー描画システムの処理[RenderingPlayerSystem.hpp]
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
#include "System/Rendering/baseRenderingsystem.hpp"

//****************************************************************
// ゲームプレイヤーの描画システム
//****************************************************************
struct RenderingGamePlayerSystem final : BaseRenderingSystem
{
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	void Rendering(entt::registry& Reg)override;
	/// <summary>
	/// 物陰描画
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Entity">プレイヤー</param>
	void RenderingShape(entt::registry& Reg, entt::entity Entity);
};