//****************************************************************
//
// マップオブジェクト描画システムの処理[RenderingMapobjectSystem.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceRenderingsystem.hpp"
#include "TransformComponent.hpp"
#include "RigitBodyComponent.hpp"

// プレイヤーのアップデート
struct RenderingMapobjectSystem : BaceRenderingSystem
{
	// コンストラクタ・デストラクタ
	RenderingMapobjectSystem() = default;
	~RenderingMapobjectSystem() = default;
	// 描画
	void Rendering(entt::registry& reg)override;
};