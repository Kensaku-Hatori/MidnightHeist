//****************************************************************
//
// 敵オブジェクト描画システムの処理[RenderingEnemySystem.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceRenderingsystem.hpp"

// 敵の描画
struct RenderingEnemySystem : BaceRenderingSystem
{
	// コンストラクタ・デストラクタ
	RenderingEnemySystem() = default;
	~RenderingEnemySystem() = default;
	// 描画
	void Rendering(entt::registry& reg)override;
	void DrawShapeShadowMap(entt::registry& Reg, entt::entity Entity);
};