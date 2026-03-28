//****************************************************************
//
// シャドウマップへの書き込みシステムの処理[RenderingToShadomapSystem.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "System/Rendering/baceRenderingsystem.hpp"

// プレイヤーのアップデート
struct RenderingToShadowmapSystem : BaceRenderingSystem
{
	// コンストラクタ・デストラクタ
	RenderingToShadowmapSystem() = default;
	~RenderingToShadowmapSystem() = default;
	// 描画
	void Rendering(entt::registry& reg)override;
};