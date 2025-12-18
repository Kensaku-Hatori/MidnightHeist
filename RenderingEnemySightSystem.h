//****************************************************************
//
// 敵の視界描画システムの処理[RenderingEnemySightSystem.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceRenderingsystem.hpp"

// 敵の描画
struct RenderingEnemySightSystem : BaceRenderingSystem
{
	// コンストラクタ・デストラクタ
	RenderingEnemySightSystem() = default;
	~RenderingEnemySightSystem() = default;
	// 描画
	void Rendering(entt::registry& reg)override;
};