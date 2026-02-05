//****************************************************************
//
// タイトルプレイヤー描画システムの処理[RenderingTitlePlayerSystem.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceRenderingsystem.hpp"

// プレイヤーのアップデート
struct RenderingTitlePlayerSystem : BaceRenderingSystem
{
	// コンストラクタ・デストラクタ
	RenderingTitlePlayerSystem() = default;
	~RenderingTitlePlayerSystem() = default;
	// 描画
	void Rendering(entt::registry& reg)override;
};