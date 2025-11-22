//****************************************************************
//
// 2D描画基本システムのクラスの処理[2Drenderingbace.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceRenderingsystem.hpp"

// 2D描画基本システムのクラス
struct Render2DSystem : BaceRenderingSystem
{
	// デストラクタ
	Render2DSystem() = default;
	// コンストラクタ
	~Render2DSystem() = default;
	// 描画
	void Rendering(entt::registry& reg)override;
};