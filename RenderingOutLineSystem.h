//****************************************************************
//
// アウトラインの描画システムの処理[RenderingOutLineSystem.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceRenderingsystem.hpp"

// プレイヤーのアップデート
struct RenderingOutLineSystem : BaceRenderingSystem
{
	// コンストラクタ・デストラクタ
	RenderingOutLineSystem() = default;
	~RenderingOutLineSystem() = default;
	// 描画
	void Rendering(entt::registry& reg)override;
};