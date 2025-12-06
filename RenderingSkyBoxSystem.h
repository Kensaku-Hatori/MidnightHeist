//****************************************************************
//
// スカイボックスの描画システムの処理[RenderingSkyBoxSystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceRenderingsystem.hpp"

// プレイヤーのアップデート
struct RenderingSkyBoxSystem : BaceRenderingSystem
{
	// コンストラクタ・デストラクタ
	RenderingSkyBoxSystem() = default;
	~RenderingSkyBoxSystem() = default;
	// 描画
	void Rendering(entt::registry& reg)override;
};