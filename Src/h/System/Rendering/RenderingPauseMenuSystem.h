//****************************************************************
//
// ポーズメニュー描画システム処理[RenderingPauseMenu.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceRenderingsystem.hpp"

// 2D描画基本システムのクラス
struct RenderingPauseMenuSystem : BaceRenderingSystem
{
	// デストラクタ
	RenderingPauseMenuSystem() : BaceRenderingSystem(false) {};
	// コンストラクタ
	~RenderingPauseMenuSystem() = default;
	// 描画
	void Rendering(entt::registry& reg)override;
};