//================================================================
//
// マップオブジェクト描画システムの処理[RenderingMapobjectSystem.hpp]
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
#include "BaseRenderingsystem.hpp"

//****************************************************************
// マップオブジェクトの描画
//****************************************************************
struct RenderingMapobjectSystem final : BaseRenderingSystem
{
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="reg">レジストリー</param>
	void Rendering(entt::registry& Reg)override;
	/// <summary>
	/// シャドウマップをすかって描画
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Entity">マップオブジェクト</param>
	void DrawUseShadowMap(entt::registry& Reg, entt::entity Entity);
};