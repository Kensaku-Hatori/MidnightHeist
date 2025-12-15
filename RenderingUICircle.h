//****************************************************************
//
// 円形UIの描画システムの処理[RenderingUICircleSystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceRenderingsystem.hpp"

// プレイヤーのアップデート
struct RenderingUICircleSystem : BaceRenderingSystem
{
	// コンストラクタ・デストラクタ
	RenderingUICircleSystem() = default;
	~RenderingUICircleSystem() = default;
	// 描画
	void Rendering(entt::registry& reg)override;
};