//================================================================
//
// 敵オブジェクト描画システムの処理[RenderingEnemySystem.hpp]
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
// 敵の描画
//****************************************************************
struct RenderingEnemySystem final : BaseRenderingSystem
{
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	void Rendering(entt::registry& Reg)override;
	/// <summary>
	/// 物陰マップに書き込む
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Entity">敵のエンティティ</param>
	void DrawShapeShadowMap(entt::registry& Reg, entt::entity Entity);
};