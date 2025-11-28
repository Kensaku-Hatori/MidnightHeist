//****************************************************************
//
// メッシュフィールド描画基本システムのクラスの処理[RenderingMeshFieldSystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceRenderingsystem.hpp"

// 2D描画基本システムのクラス
struct RenderMehLaerSystem : BaceRenderingSystem
{
	// デストラクタ
	RenderMehLaerSystem() = default;
	// コンストラクタ
	~RenderMehLaerSystem() = default;
	// 描画
	void Rendering(entt::registry& reg)override;
};