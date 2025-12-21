//****************************************************************
//
// サインカーブの描画処理[RenderingVisibleSineCurve.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceRenderingsystem.hpp"

// 敵の描画
struct RenderingVisibleSineCurveSystem : BaceRenderingSystem
{
	// コンストラクタ・デストラクタ
	RenderingVisibleSineCurveSystem() = default;
	~RenderingVisibleSineCurveSystem() = default;
	// 描画
	void Rendering(entt::registry& reg)override;
};