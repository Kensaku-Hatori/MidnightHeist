//****************************************************************
//
// 物陰への書き込みシステムの処理[RenderingToShapeShadowSystem.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceRenderingsystem.hpp"

// プレイヤーのアップデート
struct RenderingToShapeShadowSystem : BaceRenderingSystem
{
	// コンストラクタ・デストラクタ
	RenderingToShapeShadowSystem() = default;
	~RenderingToShapeShadowSystem() = default;
	// 描画
	void Rendering(entt::registry& reg)override;
};